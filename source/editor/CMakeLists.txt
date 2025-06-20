# Copyright (c) 2025 by Tim Janke. All rights reserved.

# set up the include directory for the editor
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../engine")

# if we are on macOS we have a different build process than on other platforms
if (APPLE)
    add_executable(watschel MACOSX_BUNDLE watschel/platform/macos/main.mm watschel/platform/macos/AppDelegate.hxx watschel/platform/macos/AppDelegate.mm ${WATSCHEL_SOURCE_CORE})
    set_target_properties(watschel PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/plist.in)
elseif(WIN32)
    add_executable(watschel WIN32 watschel/platform/windows/main.cxx ${WATSCHEL_SOURCE_CORE} ${WATSCHEL_LAYERS_SOURCE_CORE} ${WATSCHEL_PANELS_SOURCE_CORE})
else()
    add_executable(watschel watschel/platform/linux/main.cxx ${WATSCHEL_SOURCE_CORE} ${WATSCHEL_LAYERS_SOURCE_CORE} ${WATSCHEL_PANELS_SOURCE_CORE})
endif()

#
set(WATSCHEL_SOURCE_CORE ${WATSCHEL_SOURCE_CORE} watschel/WatschelApp.hxx watschel/WatschelApp.cxx)

target_link_libraries(watschel adelie_engine ${CMAKE_DL_LIBS} ${WATCHEL_LIBRARIES_LINUX} ${WATCHEL_LIBRARIES_MACOS} ${WATCHEL_LIBRARIES_WINDOWS})
install(TARGETS watschel RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static BUNDLE DESTINATION bundle)