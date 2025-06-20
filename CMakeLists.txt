# Copyright (c) 2025 by Tim Janke. All rights reserved.

# ensure we can find packages installed through vcpkg
if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif ()

# ensure vcpkg uses the correct architecture
if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif ()

# configure the basic project, we need a cmake version which supports C++20
cmake_minimum_required(VERSION 3.31.6)
project(Adelie)

# ensure the compilation database is always generated
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# configure the used C++ standard and configure the folder where the CMake helper scripts are stored
set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts/cmake")
include("${CMAKE_SOURCE_DIR}/scripts/cmake/InstallHooks.cmake")

############################## Compiler Settings based on the Platform we are running on ##############################

# if we are on Linux or macOS, we just support clang, on Windows only MSVC
if (UNIX)
    # if the wrong compiler is used, terminate immediately
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "The only supported C++ compiler is clang")
    endif ()
    if (NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "The only supported C compiler is clang")
    endif ()

    # enable all possible warnings and be pedantic about it
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-unknown-pragmas")

    # define the platform guard based on if we're on Apple or not (Linux)
    if (NOT APPLE)
        add_compile_definitions("ADELIE_PLATFORM_LINUX")
    else ()
        set(CMAKE_FIND_FRAMEWORK FIRST)
        add_compile_definitions("ADELIE_PLATFORM_MACOS")
    endif ()
elseif (WIN32)
    # if the wrong compiler is used, terminate immediately
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "The only supported C++ compiler is the Microsoft Visual C++ compiler")
    endif ()
    if (NOT CMAKE_C_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "The only supported C compiler is the Microsoft Visual C compiler")
    endif ()

    # ensure the compiler supports the latest C++ standard (until it is completed and the /std:c++20 can be used)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

    # add some required compiler defines for Windows
    add_compile_definitions("NOMINMAX")
    add_compile_definitions("WIN32_LEAN_AND_MEAN")
    add_compile_definitions("UNICODE")
    add_compile_definitions("ADELIE_PLATFORM_WINDOWS")

    # ensure that we build an executable with an wWinMain entry point on Windows
    set(CMAKE_WIN32_EXECUTABLE ON)
else ()
    MESSAGE(FATAL_ERROR "The used platform is currently not supported")
endif ()

############################## Extraction of Version Information from Git and providing them to the Code ##############################

# get the version information from git (depending on the last version tag)
include(git/GetGitRevisionDescription)
git_describe(VERSION "--tags")
git_local_changes(VERSION_STATE)
if (VERSION_STATE STREQUAL "DIRTY")
    set(VERSION_IS_DIRTY 1)
endif ()

# parse the version information and split them into pieces
string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# get the branch information for the current version
git_describe(VERSION "--all")
set(VERSION_BRANCH "${VERSION}")

# generate the file with the version information
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/VersionInformation.cxx")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/source/engine/adelie/VersionInformation.cxx.in ${version_file})

############################## Library Dependencies for the whole project (including all Tools + Engine) ##############################

# setup some properties for the Boost libraries
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# packages which are always required
find_package(Boost 1.82.0 REQUIRED COMPONENTS thread)

# TODO: should be selected if Vulkan rendering is configured
find_package(Vulkan REQUIRED)
set(ADELIE_VULKAN_LINK_TARGET Vulkan::Vulkan)

# the libraries which are only required on Linux (e.g. the corresponding window APIs)
if (UNIX AND NOT APPLE)
    find_package(XCB REQUIRED)
    
    # Find Wayland and wayland-scanner for protocol generation
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
    pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
    
    # Find wayland-scanner
    pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)
    if(NOT WAYLAND_SCANNER)
        find_program(WAYLAND_SCANNER NAMES wayland-scanner)
    endif()
    if(NOT WAYLAND_SCANNER)
        message(FATAL_ERROR "wayland-scanner not found")
    endif()
    
    # Get wayland-protocols path
    pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)
    if(NOT WAYLAND_PROTOCOLS_DIR)
        set(WAYLAND_PROTOCOLS_DIR "/usr/share/wayland-protocols")
    endif()
    
    # Set up protocol generation directory
    set(WAYLAND_PROTOCOLS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols")
    file(MAKE_DIRECTORY ${WAYLAND_PROTOCOLS_OUTPUT_DIR})
    
    # Generate xdg-shell protocol files
    set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
    set(XDG_SHELL_CLIENT_HEADER "${WAYLAND_PROTOCOLS_OUTPUT_DIR}/xdg-shell-client-protocol.h")
    set(XDG_SHELL_PRIVATE_CODE "${WAYLAND_PROTOCOLS_OUTPUT_DIR}/xdg-shell-protocol.c")
    
    add_custom_command(
        OUTPUT ${XDG_SHELL_CLIENT_HEADER}
        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_CLIENT_HEADER}
        DEPENDS ${XDG_SHELL_XML}
        COMMENT "Generating xdg-shell client header"
    )
    
    add_custom_command(
        OUTPUT ${XDG_SHELL_PRIVATE_CODE}
        COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_PRIVATE_CODE}
        DEPENDS ${XDG_SHELL_XML}
        COMMENT "Generating xdg-shell private code"
    )
    
    # Create a custom target for protocol generation
    add_custom_target(wayland-protocols-generate
        DEPENDS ${XDG_SHELL_CLIENT_HEADER} ${XDG_SHELL_PRIVATE_CODE}
    )

    set(WATCHEL_LIBRARIES_LINUX ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES})
    set(ADELIE_WAYLAND_PROTOCOLS_SOURCES ${XDG_SHELL_PRIVATE_CODE})
    set(ADELIE_WAYLAND_PROTOCOLS_INCLUDE_DIR ${WAYLAND_PROTOCOLS_OUTPUT_DIR})
endif()

# the frameworks which are only required on MacOS to for platform specific stuff (like creating the rendering window)
if (APPLE)
    set(ADELIE_LIBRARIES_MACOS ${ADELIE_LIBRARIES_MACOS} "$<LINK_LIBRARY:FRAMEWORK,Cocoa>")
    set(ADELIE_LIBRARIES_MACOS ${ADELIE_LIBRARIES_MACOS} "$<LINK_LIBRARY:FRAMEWORK,QuartzCore>")

    set(WATCHEL_LIBRARIES_MACOS ${WATCHEL_LIBRARIES_MACOS} "$<LINK_LIBRARY:FRAMEWORK,AppKit>")
endif()

############################## Scripts for Making the Development Process Easier ##############################

# install the pre-commit hooks to ensure we contain a formatted code-base
install_hooks()

# Link the compile command database to the binary directory. This is only done on Unix-based systems. On Windows
# the supported development is with Visual Studio and the compile database is here not required to be linked
# into the root directory
if (UNIX)
    set(COMPILE_COMMANDS_SRC ${CMAKE_BINARY_DIR}/compile_commands.json)
    set(COMPILE_COMMANDS_LINK ${CMAKE_SOURCE_DIR}/compile_commands.json)
    message(STATUS "Linking compile command database to ${COMPILE_COMMANDS_LINK}")
    file(CREATE_LINK ${COMPILE_COMMANDS_SRC} ${COMPILE_COMMANDS_LINK} SYMBOLIC)
endif()

############################## The Paths which should be Checked for more CMakeLists.txt Files ##############################

add_subdirectory("source")