// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <Adelie.hxx>

using adelie::core::renderer::WindowFactory;

auto main(int /*unused*/, char** /*unused*/) -> int {
    auto window = WindowFactory::createWindow();
    window->createWindow(1920, 1080, "Watschel");
}
