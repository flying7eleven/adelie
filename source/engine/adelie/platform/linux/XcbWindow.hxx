// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_PLATFORM_LINUX_XCBWINDOW_HXX__)
    #define __ADELIE_PLATFORM_LINUX_XCBWINDOW_HXX__

    #include <xcb/xcb.h>

    #include <adelie/core/renderer/WindowInterface.hxx>

namespace adelie::platform {

    class XcbWindow : public ::adelie::core::renderer::WindowInterface {
        public:
            XcbWindow();
            ~XcbWindow() override;

            void createWindow(const int& width, const int& height, const char* title) override;
            void destroyWindow() override;
            void* getNativeWindowHandle() const override;
            void* getNativeDisplayHandle() const override;
            void getWindowSize(int& width, int& height) const override;
            bool shouldClose() const override;
            void pollEvents() override;

        private:
            xcb_connection_t* connection;
            xcb_window_t window;
            bool shouldCloseFlag;
    };

} /* namespace adelie::platform */

#endif /* if !defined(__ADELIE_PLATFORM_LINUX_XCBWINDOW_HXX__) */