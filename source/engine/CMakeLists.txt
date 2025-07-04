# Copyright (c) 2025 by Tim Janke. All rights reserved.

# set up the include directory for the engine
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

#
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} Adelie.hxx)
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/VersionInformation.hxx ${version_file})
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/core/Input.hxx)
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/core/Assert.hxx)
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/core/Layer.hxx adelie/core/Layer.cxx)
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/core/LayerStack.hxx adelie/core/LayerStack.cxx)
set(ADELIE_SOURCE_CORE ${ADELIE_SOURCE_CORE} adelie/core/Timestep.hxx)

#
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/EventDispatcher.hxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/Event.hxx adelie/core/events/Event.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/KeyEvent.hxx adelie/core/events/KeyEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/MouseMovedEvent.hxx adelie/core/events/MouseMovedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/MouseScrolledEvent.hxx adelie/core/events/MouseScrolledEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/MouseButtonEvent.hxx adelie/core/events/MouseButtonEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/MouseButtonPressedEvent.hxx adelie/core/events/MouseButtonPressedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/MouseButtonReleasedEvent.hxx adelie/core/events/MouseButtonReleasedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/KeyPressedEvent.hxx adelie/core/events/KeyPressedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/KeyReleasedEvent.hxx adelie/core/events/KeyReleasedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/KeyTypedEvent.hxx adelie/core/events/KeyTypedEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/WindowCloseEvent.hxx adelie/core/events/WindowCloseEvent.cxx)
set(ADELIE_SOURCE_CORE_EVENT ${ADELIE_SOURCE_CORE_EVENT} adelie/core/events/WindowResizeEvent.hxx adelie/core/events/WindowResizeEvent.cxx)

#
set(ADELIE_SOURCE_CORE_RENDERER ${ADELIE_SOURCE_CORE_RENDERER} adelie/core/renderer/WindowFactory.hxx adelie/core/renderer/WindowFactory.cxx)
set(ADELIE_SOURCE_CORE_RENDERER ${ADELIE_SOURCE_CORE_RENDERER} adelie/core/renderer/WindowInterface.hxx)
set(ADELIE_SOURCE_CORE_RENDERER ${ADELIE_SOURCE_CORE_RENDERER} adelie/core/renderer/Renderer.hxx adelie/core/renderer/Renderer.cxx)

#
set(ADELIE_SOURCE_EXCEPTION ${ADELIE_SOURCE_EXCEPTION} adelie/exception/IOException.hxx adelie/exception/IOException.cxx)
set(ADELIE_SOURCE_EXCEPTION ${ADELIE_SOURCE_EXCEPTION} adelie/exception/NotImplementedException.hxx adelie/exception/NotImplementedException.cxx)
set(ADELIE_SOURCE_EXCEPTION ${ADELIE_SOURCE_EXCEPTION} adelie/exception/RuntimeException.hxx adelie/exception/RuntimeException.cxx)
set(ADELIE_SOURCE_EXCEPTION ${ADELIE_SOURCE_EXCEPTION} adelie/exception/VulkanRuntimeException.hxx adelie/exception/VulkanRuntimeException.cxx)

#
set(ADELIE_SOURCE_RENDERER_VULKAN ${ADELIE_SOURCE_RENDERER_VULKAN} adelie/renderer/vulkan/VulkanVertex.hxx)
set(ADELIE_SOURCE_RENDERER_VULKAN ${ADELIE_SOURCE_RENDERER_VULKAN} adelie/renderer/vulkan/VulkanRenderer.hxx adelie/renderer/vulkan/VulkanRenderer.cxx)
set(ADELIE_SOURCE_RENDERER_VULKAN ${ADELIE_SOURCE_RENDERER_VULKAN} adelie/renderer/vulkan/VulkanShaderManager.hxx adelie/renderer/vulkan/VulkanShaderManager.cxx)
set(ADELIE_SOURCE_RENDERER_VULKAN ${ADELIE_SOURCE_RENDERER_VULKAN} adelie/renderer/vulkan/VulkanExtensionManager.hxx adelie/renderer/vulkan/VulkanExtensionManager.cxx)

# create a list of all source files of the I/O module of the engine
set(ADELIE_SOURCE_IO ${ADELIE_SOURCE_IO} adelie/io/Logger.hxx adelie/io/Logger.cxx)

# the render target implementation varies on the used platform
if (UNIX AND NOT APPLE)
    # if we use Linux, the used backend for creating a window will be XCB
    add_compile_definitions("VK_USE_PLATFORM_XCB_KHR")
    add_compile_definitions("VK_USE_PLATFORM_WAYLAND_KHR")

    # list all files for the render target which are specific to Linux
    set(ADELIE_SOURCE_PLATFORM ${ADELIE_SOURCE_PLATFORM} adelie/platform/linux/XcbWindow.hxx adelie/platform/linux/XcbWindow.cxx)
    set(ADELIE_SOURCE_PLATFORM ${ADELIE_SOURCE_PLATFORM} adelie/platform/linux/WaylandWindow.hxx adelie/platform/linux/WaylandWindow.cxx)
    
    # Add Wayland protocol generated sources
    set(ADELIE_SOURCE_PLATFORM ${ADELIE_SOURCE_PLATFORM} ${ADELIE_WAYLAND_PROTOCOLS_SOURCES})
elseif (APPLE)
    # if we use macOS, the used backend for creating a Vulkan window will be MoltenVK
    add_compile_definitions("VK_USE_PLATFORM_MACOS_MVK")

    # list all files for the render target which are specific to macOS
    set(ADELIE_SOURCE_PLATFORM ${ADELIE_SOURCE_PLATFORM} adelie/platform/macos/CocoaWindow.hxx adelie/platform/macos/CocoaWindow.mm)
elseif (WIN32)
    # if we use Windows, there is only one valid window backend
    add_compile_definitions("VK_USE_PLATFORM_WIN32_KHR")

    # list all files for the render target which are specific to Windows
    set(ADELIE_SOURCE_PLATFORM ${ADELIE_SOURCE_PLATFORM} adelie/platform/windows/Win32Window.hxx adelie/platform/windows/Win32Window.cxx)
endif ()

#
add_compile_definitions("ADELIE_BUILD_LIBRARY")

# configure the static library for the engine
add_library(adelie_engine SHARED ${ADELIE_SOURCE_CORE} ${ADELIE_SOURCE_CORE_RENDERER} ${ADELIE_SOURCE_CORE_EVENT} ${ADELIE_SOURCE_IO} ${ADELIE_SOURCE_PLATFORM} ${ADELIE_SOURCE_RENDERER_VULKAN} ${ADELIE_SOURCE_EXCEPTION})

# Add include directories for Wayland protocols on Linux
if (UNIX AND NOT APPLE)
    target_include_directories(adelie_engine PRIVATE ${ADELIE_WAYLAND_PROTOCOLS_INCLUDE_DIR})
    add_dependencies(adelie_engine wayland-protocols-generate)
endif()

target_link_libraries(adelie_engine Boost::disable_autolinking Boost::thread ${ADELIE_LIBRARIES_MACOS} ${ADELIE_LIBRARIES_WINDOWS} ${ADELIE_LIBRARIES_LINUX} ${ADELIE_VULKAN_LINK_TARGET})
set_target_properties(adelie_engine PROPERTIES OUTPUT_NAME adelie SOVERSION 1)