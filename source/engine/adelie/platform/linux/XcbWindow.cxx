// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include "XcbWindow.hxx"

#include <adelie/exception/RuntimeException.hxx>
#include <cstring>
#include <stdexcept>

using adelie::exception::RuntimeException;
using adelie::platform::XcbWindow;

XcbWindow::XcbWindow() : connection(nullptr), window(0), shouldCloseFlag(false) {
    connection = xcb_connect(nullptr, nullptr);
    if (xcb_connection_has_error(connection)) {
        throw RuntimeException("Failed to connect to X server");
    }
}

XcbWindow::~XcbWindow() {
    destroyWindow();
    if (connection) {
        xcb_disconnect(connection);
    }
}

void XcbWindow::createWindow(const int& width, const int& height, const char* title) {
    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_t* screen = xcb_setup_roots_iterator(setup).data;

    window = xcb_generate_id(connection);

    uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[] = {screen->black_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY};

    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, width, height, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

    // Set window title
    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);

    xcb_map_window(connection, window);
    xcb_flush(connection);
}

void XcbWindow::destroyWindow() {
    if (window) {
        xcb_destroy_window(connection, window);
        window = 0;
    }
}

void* XcbWindow::getNativeWindowHandle() const {
    return reinterpret_cast<void*>(window);
}

void* XcbWindow::getNativeDisplayHandle() const {
    return connection;
}

void XcbWindow::getWindowSize(int& width, int& height) const {
    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(connection, window);
    xcb_get_geometry_reply_t* reply = xcb_get_geometry_reply(connection, cookie, nullptr);
    if (reply) {
        width = reply->width;
        height = reply->height;
        free(reply);
    }
}

bool XcbWindow::shouldClose() const {
    return shouldCloseFlag;
}

void XcbWindow::pollEvents() {
    xcb_generic_event_t* event;
    while ((event = xcb_poll_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
                shouldCloseFlag = true;
                break;
            case XCB_CONFIGURE_NOTIFY:
                // Handle window resize if needed
                break;
        }
        free(event);
    }
}