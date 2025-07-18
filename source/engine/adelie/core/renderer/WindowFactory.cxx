// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/renderer/Renderer.hxx>
#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <stdexcept>

using adelie::core::renderer::Renderer;
using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowInterface;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;

#if defined(ADELIE_PLATFORM_LINUX)
    #include <adelie/platform/linux/WaylandWindow.hxx>
    #include <adelie/platform/linux/XcbWindow.hxx>

using adelie::platform::WaylandWindow;
using adelie::platform::XcbWindow;
#endif /* defined(ADELIE_PLATFORM_LINUX) */

#if defined(ADELIE_PLATFORM_WINDOWS)
    #include <adelie/platform/windows/Win32Window.hxx>

using adelie::platform::Win32Window;
#endif /* defined(ADELIE_PLATFORM_WINDOWS) */

#if defined(ADELIE_PLATFORM_MACOS)
    #include <adelie/platform/macos/CocoaWindow.hxx>
using adelie::platform::CocoaWindow;
#endif /* defined(ADELIE_PLATFORM_MACOS) */

auto WindowFactory::getWindowType() -> WindowType {
    static WindowType windowType = detectWindowType();
    return windowType;
}

WindowType WindowFactory::detectWindowType() {
#if defined(ADELIE_PLATFORM_LINUX)
    // Check for Wayland first (takes precedence if both are available)
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
    if (waylandDisplay && waylandDisplay[0] != '\0') {
        AdelieLogDebug("Found WAYLAND_DISPLAY environment variable, thus trying to create a Wayland window");
        return WindowType::WAYLAND_API;
    }

    // Fall back to X11 if DISPLAY is set
    const char* x11Display = std::getenv("DISPLAY");
    if (x11Display && x11Display[0] != '\0') {
        AdelieLogDebug("Found DISPLAY and no WAYLAND_DISPLAY environment variable, thus trying to create X11 window");
        return WindowType::XCB_API;
    }

    // if neither environment variable is set, try Wayland first as it's more modern the actual window creation will fail
    // gracefully if the display server is not available
    return WindowType::WAYLAND_API;
#elif defined(ADELIE_PLATFORM_WINDOWS)
    AdelieLogDebug("Running on Windows, thus trying to create a window using the WIN32 API");
    return WindowType::WIN32_API;
#elif defined(ADELIE_PLATFORM_MACOS)
    AdelieLogDebug("Running on macOS, thus trying to create a window using the Cocoa API");
    return WindowType::COCOA_API;
#else
    throw RuntimeException("Platform not supported for automatic window type detection");
#endif
}

std::shared_ptr<WindowInterface> WindowFactory::createWindow() {
    std::shared_ptr<WindowInterface> windowInterface = nullptr;
    switch (getWindowType()) {
        case WindowType::XCB_API:
#if defined(ADELIE_PLATFORM_LINUX)
            windowInterface = std::make_shared<XcbWindow>();
            break;
#else
            throw RuntimeException("XCB window type not supported on this platform");
#endif /* defined(ADELIE_PLATFORM_LINUX) */
        case WindowType::WAYLAND_API:
#if defined(ADELIE_PLATFORM_LINUX)
            windowInterface = std::make_shared<WaylandWindow>();
            break;
#else
            throw RuntimeException("WaylandWindow window type not supported on this platform");
#endif /* defined(ADELIE_PLATFORM_LINUX) */
        case WindowType::WIN32_API:
#if defined(ADELIE_PLATFORM_WINDOWS)
            windowInterface = std::make_shared<Win32Window>();
            break;
#else
            throw RuntimeException("Win32 window type not supported on this platform");
#endif /* defined(ADELIE_PLATFORM_WINDOWS) */
        case WindowType::COCOA_API:
#if defined(ADELIE_PLATFORM_MACOS)
            windowInterface = std::make_shared<CocoaWindow>();
            break;
#else
            throw RuntimeException("Cocoa window type not supported on this platform");
#endif /* defined(ADELIE_PLATFORM_MACOS) */
        default:
            throw RuntimeException("Unsupported window type");
    }

    return windowInterface;
}