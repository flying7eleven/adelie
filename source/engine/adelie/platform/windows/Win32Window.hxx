// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_PLATFORM_WINDOWS_WIN32WINDOW_HXX__)
    #define __ADELIE_PLATFORM_WINDOWS_WIN32WINDOW_HXX__

    #include <windows.h>

    #include <adelie/core/renderer/WindowInterface.hxx>

namespace adelie::platform {

    class Win32Window : public ::adelie::core::renderer::WindowInterface {
        public:
            Win32Window();
            ~Win32Window() override;

            void createWindow(const int& width, const int& height, const char* title) override;
            void destroyWindow() override;
            void* getNativeWindowHandle() const override;
            void* getNativeDisplayHandle() const override;
            void getWindowSize(int& width, int& height) const override;
            bool shouldClose() const override;
            void pollEvents() override;

        private:
            static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
            HWND hwnd;
            bool shouldCloseFlag;
    };

} /* namespace adelie::platform */

#endif /* if !defined(__ADELIE_PLATFORM_WINDOWS_WIN32WINDOW_HXX__) */