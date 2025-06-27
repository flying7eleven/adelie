// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <Adelie.hxx>
#include <adelie/core/renderer/Renderer.hxx>

using adelie::core::renderer::Renderer;
using adelie::core::renderer::WindowFactory;

auto main(int /*unused*/, char** /*unused*/) -> int {
    auto window = WindowFactory::createWindow();
    window->createWindow(1920, 1080, "Watschel");

    Renderer::setAPI(Renderer::API::Vulkan);
    Renderer::initialize(window);
}
