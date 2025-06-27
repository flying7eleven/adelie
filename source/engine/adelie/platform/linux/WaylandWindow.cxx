// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include "WaylandWindow.hxx"

#include <xdg-shell-client-protocol.h>

#include <adelie/core/Assert.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <cstring>
#include <stdexcept>

using adelie::exception::RuntimeException;
using adelie::platform::WaylandWindow;

namespace adelie::platform {
    void registry_handle_global(void* data, wl_registry* registry, const uint32_t id, const char* interface, uint32_t /*version*/) {
        auto* window = static_cast<WaylandWindow*>(data);

        if (strcmp(interface, wl_compositor_interface.name) == 0) {
            window->compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, id, &wl_compositor_interface, 4));
        } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
            window->xdg_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
        }
    }

    void registry_handle_global_remove(void* /*data*/, wl_registry* /*registry*/, uint32_t /*name*/) {}

    constexpr wl_registry_listener registry_listener = {registry_handle_global, registry_handle_global_remove};

    void xdg_wm_base_ping(void* /*data*/, xdg_wm_base* xdg_wm_base, const uint32_t serial) {
        xdg_wm_base_pong(xdg_wm_base, serial);
    }

    constexpr xdg_wm_base_listener xdg_wm_base_listener = {xdg_wm_base_ping};

    auto xdg_surface_configure(void* /*data*/, xdg_surface* xdg_surface, const uint32_t serial) -> void {
        xdg_surface_ack_configure(xdg_surface, serial);
    }

    constexpr xdg_surface_listener xdg_surface_listener = {xdg_surface_configure};

    void xdg_toplevel_configure(void* data, xdg_toplevel* /*xdg_toplevel*/, const int32_t width, const int32_t height, wl_array* /*states*/) {
        auto* window = static_cast<WaylandWindow*>(data);
        if (width > 0 && height > 0) {
            window->windowWidth = width;
            window->windowHeight = height;
        }
    }

    void xdg_toplevel_close(void* data, xdg_toplevel* /*xdg_toplevel*/) {
        auto* window = static_cast<WaylandWindow*>(data);
        window->shouldCloseFlag = true;
    }

    constexpr xdg_toplevel_listener xdg_toplevel_listener = {xdg_toplevel_configure, xdg_toplevel_close, nullptr, nullptr};
} /* namespace adelie::platform */

WaylandWindow::WaylandWindow()
    : compositor(nullptr), xdg_wm_base(nullptr), windowWidth(0), windowHeight(0), shouldCloseFlag(false), display(nullptr), registry(nullptr), surface(nullptr), xdg_surface(nullptr), xdg_toplevel(nullptr) {
    display = wl_display_connect(nullptr);
    if (!display) {
        throw RuntimeException("Failed to connect to Wayland display");
    }
}

WaylandWindow::~WaylandWindow() {
    WaylandWindow::destroyWindow();
    if (display) {
        wl_display_disconnect(display);
    }
}

void WaylandWindow::createWindow(const int& width, const int& height, const char* title) {
    // get registry and bind to global objects and wait for registry events
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);

    // handle if we could not get the handle to the compositor
    if (!compositor) {
        AdelieLogError("Could not get handle to the Wayland compositor");
        throw RuntimeException("Failed to bind Wayland compositor");
    }

    // handle if we could not get the handle to the XDG WM Base
    if (!xdg_wm_base) {
        AdelieLogError("Could not get handle base address to the XDG WM structure");
        throw RuntimeException("Could not get handle base address to the XDG WM structure");
    }

    xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, this);

    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        AdelieLogError("Could not create a new Wayland surface");
        throw RuntimeException("Failed to create Wayland surface");
    }

    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    if (!xdg_surface) {
        AdelieLogError("Could not create a new XDG surface");
        throw RuntimeException("Failed to create XDG surface");
    }
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);

    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    if (!xdg_toplevel) {
        AdelieLogError("Could not register the a top-level window for this application");
        throw RuntimeException("Could not register the a top-level window for this application");
    }
    xdg_toplevel_add_listener(xdg_toplevel, &xdg_toplevel_listener, this);

    // set window title
    xdg_toplevel_set_title(xdg_toplevel, title);

    // set the initial window size
    windowWidth = width;
    windowHeight = height;

    // commit the changes and wait for events
    wl_surface_commit(surface);
    wl_display_roundtrip(display);
    AdelieLogDebug("Successfully initialized Wayland window");
}

void WaylandWindow::destroyWindow() {
    if (xdg_toplevel) {
        xdg_toplevel_destroy(xdg_toplevel);
        xdg_toplevel = nullptr;
    }
    if (xdg_surface) {
        xdg_surface_destroy(xdg_surface);
        xdg_surface = nullptr;
    }
    if (surface) {
        wl_surface_destroy(surface);
        surface = nullptr;
    }
    if (xdg_wm_base) {
        xdg_wm_base_destroy(xdg_wm_base);
        xdg_wm_base = nullptr;
    }
    if (compositor) {
        wl_compositor_destroy(compositor);
        compositor = nullptr;
    }
    if (registry) {
        wl_registry_destroy(registry);
        registry = nullptr;
    }
    AdelieLogDebug("Cleaned up all Wayland related resources");
}

void* WaylandWindow::getNativeWindowHandle() const {
    AdelieCoreAssert(surface != nullptr, "no valid surface to return");
    return surface;
}

void* WaylandWindow::getNativeDisplayHandle() const {
    AdelieCoreAssert(display != nullptr, "no valid display to return");
    return display;
}

void WaylandWindow::getWindowSize(int& width, int& height) const {
    width = windowWidth;
    height = windowHeight;
}

bool WaylandWindow::shouldClose() const {
    return shouldCloseFlag;
}

void WaylandWindow::pollEvents() {
    wl_display_dispatch_pending(display);
    wl_display_flush(display);
}