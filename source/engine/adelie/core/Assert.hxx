// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_CORE_ASSERT_HXX__)
    #define __ADELIE_CORE_ASSERT_HXX__

    #include <adelie/io/Logger.hxx>

    // define the required assertion macros
    #if !defined(NDEBUG)
template <typename T>
constexpr auto AdelieAssert(bool condition, T message) {
    if (!(condition)) {
        AdelieLogError("Assertion failed: {}", message);
        #if defined(ADELIE_PLATFORM_WINDOWS)
        __debugbreak();
        #elif defined(ADELIE_PLATFORM_LINUX)
        __builtin_trap();
        #elif defined(ADELIE_PLATFORM_MACOS)
        __builtin_trap();
        #else
            #error The current platform is not supported
        #endif
    }
}

template <typename T>
constexpr auto AdelieCoreAssert(bool condition, T message) {
    if (!(condition)) {
        AdelieLogError("Core assertion failed: {}", message);
        #if defined(ADELIE_PLATFORM_WINDOWS)
        __debugbreak();
        #elif defined(ADELIE_PLATFORM_LINUX)
        __builtin_trap();
        #elif defined(ADELIE_PLATFORM_MACOS)
        __builtin_trap();
        #else
            #error The current platform is not supported
        #endif
    }
}
    #else
        #define AdelieAssert(condition, message)
        #define AdelieCoreAssert(condition, message)
    #endif

#endif /* if !defined(__ADELIE_CORE_ASSERT_HXX__) */
