// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_RENDERER_WINDOWFACTORY_HXX__)
    #define __ADELIE_CORE_RENDERER_WINDOWFACTORY_HXX__

    #include <adelie/adelie.hxx>
    #include <adelie/core/renderer/WindowInterface.hxx>
    #include <memory>

namespace adelie::core::renderer {

    enum class WindowType {
        // Use the XCB API (X11) for creating and managing windows.
        XCB_API,
        // Use the Wayland API for creating and managing windows.
        WAYLAND_API,
        // Use the WIN32 API for creating and managing windows.
        WIN32_API,
        // Use the Cocoa API for creating and managing windows.
        COCOA_API,
    };

    class WindowFactory {
        public:
            static ADELIE_API std::shared_ptr<WindowInterface> createWindow();

            static ADELIE_API auto getWindowType() -> WindowType;

        private:
            // Automatically detect the best window type for the current platform/environment
            static ADELIE_API WindowType detectWindowType();
    };

} /* namespace adelie::core::renderer */

#endif /* if !defined(__ADELIE_CORE_RENDERER_WINDOWFACTORY_HXX__) */