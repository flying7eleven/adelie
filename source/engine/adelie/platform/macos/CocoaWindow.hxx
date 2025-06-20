// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_PLATFORM_MACOS_COCOAWINDOW_HXX__)
    #define __ADELIE_PLATFORM_MACOS_COCOAWINDOW_HXX__

    #include <objc/objc.h>

    #include <adelie/core/renderer/WindowInterface.hxx>

namespace adelie::platform {

    class CocoaWindow : public ::adelie::core::renderer::WindowInterface {
        public:
            CocoaWindow();
            ~CocoaWindow() override;

            void createWindow(const int& width, const int& height, const char* title) override;
            void destroyWindow() override;
            void* getNativeWindowHandle() const override;
            void* getNativeDisplayHandle() const override;
            void getWindowSize(int& width, int& height) const override;
            bool shouldClose() const override;
            void pollEvents() override;

        private:
            id window;    // NSWindow*
            id view;      // NSView*
            id delegate;  // NSWindowDelegate*
            bool shouldCloseFlag;
    }; /* class CocoaWindow */

} /* namespace adelie::platform */

#endif /* if !defined(__ADELIE_PLATFORM_MACOS_COCOAWINDOW_HXX__) */