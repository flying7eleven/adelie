// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_PLATFORM_LINUX_WAYLANDWINDOW_HXX__)
    #define __ADELIE_PLATFORM_LINUX_WAYLANDWINDOW_HXX__

    #include <wayland-client.h>

    #include <adelie/core/renderer/WindowInterface.hxx>

// Forward declarations for xdg-shell types
struct xdg_wm_base;
struct xdg_surface;
struct xdg_toplevel;

namespace adelie::platform {

    class WaylandWindow : public ::adelie::core::renderer::WindowInterface {
        public:
            WaylandWindow();
            ~WaylandWindow() override;

            void createWindow(const int& width, const int& height, const char* title) override;
            void destroyWindow() override;
            void* getNativeWindowHandle() const override;
            void* getNativeDisplayHandle() const override;
            void getWindowSize(int& width, int& height) const override;
            bool shouldClose() const override;
            void pollEvents() override;

            // Public members needed by callback functions
            struct wl_compositor* compositor;
            struct xdg_wm_base* xdg_wm_base;
            int windowWidth;
            int windowHeight;
            bool shouldCloseFlag;

        private:
            struct wl_display* display;
            struct wl_registry* registry;
            struct wl_surface* surface;
            struct xdg_surface* xdg_surface;
            struct xdg_toplevel* xdg_toplevel;
    };

} /* namespace adelie::platform */

#endif /* if !defined(__ADELIE_PLATFORM_LINUX_WAYLANDWINDOW_HXX__) */