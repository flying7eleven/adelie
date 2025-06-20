// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_RENDERER_WINDOWINTERFACE_HXX__)
    #define __ADELIE_CORE_RENDERER_WINDOWINTERFACE_HXX__

namespace adelie::core::renderer {

    class WindowInterface {
        public:
            virtual ~WindowInterface() = default;

            virtual void createWindow(const int& width, const int& height, const char* title) = 0;
            virtual void destroyWindow() = 0;
            virtual void* getNativeWindowHandle() const = 0;
            virtual void* getNativeDisplayHandle() const = 0;
            virtual void getWindowSize(int& width, int& height) const = 0;
            virtual bool shouldClose() const = 0;
            virtual void pollEvents() = 0;
    };

} /* namespace adelie::core::renderer */

#endif /* if !defined(__ADELIE_CORE_RENDERER_WINDOWINTERFACE_HXX__) */