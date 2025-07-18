// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_INTERNAL_HXX__)
    #define __ADELIE_INTERNAL_HXX__

    #include <stb/stb_image.h>  // loading images from disk / memory

    #include <glm/glm.hpp>  // all math-related stuff e.g., glm::vec3, glm::mat4x4
    #include <memory>       // std::unique_ptr, std::shared_ptr

    // define the corresponding debug definitions which can be used to determine the build type
    #if !defined(NDEBUG)
        #define ADELIE_BUILD_TYPE_DEBUG  // a complete debug build
    #else
        #define ADELIE_BUILD_TYPE_RELEASE  // optimized build which still can contain dev tools
    // #define ADELIE_BUILD_TYPE_PRODUCTION  // build for other people (when it's ready xD)
    #endif

    // define the required attributes for the object to be exported or not
    #if defined(ADELIE_PLATFORM_WINDOWS)
        #if defined(ADELIE_BUILD_LIBRARY)
            #define ADELIE_API __declspec(dllexport)
        #else
            #define ADELIE_API __declspec(dllimport)
        #endif
    #elif defined(ADELIE_PLATFORM_LINUX)
        #define ADELIE_API __attribute__((visibility("default")))
    #elif defined(ADELIE_PLATFORM_MACOS)
        #define ADELIE_API __attribute__((visibility("default")))
    #else
        #error "The current platform is not supported!"
    #endif

    // ensure the `linux` macro is not defined since it is used in namespace names
    // and `__linux__` is used to check the corresponding platform
    #if defined(__clang__) && defined(linux)
        #undef linux
    #endif

#endif /* if !defined( __ADELIE_INTERNAL_HXX__ ) */
