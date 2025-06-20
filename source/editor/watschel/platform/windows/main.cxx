// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <windows.h>

#include <Adelie.hxx>
using adelie::core::renderer::WindowFactory;

auto WINAPI wWinMain(HINSTANCE /*unused*/, HINSTANCE /*unused*/, PWSTR /*unused*/, int /*unused*/) -> int {
    auto window = WindowFactory::createWindow();
    window->createWindow(1920, 1080, "Watschel");
}
