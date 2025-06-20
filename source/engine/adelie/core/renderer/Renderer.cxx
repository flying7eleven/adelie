// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/renderer/Renderer.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanRenderer.hxx>

using adelie::core::renderer::Renderer;
using adelie::exception::RuntimeException;
using adelie::renderer::vulkan::VulkanRenderer;

Renderer::API Renderer::sAPI = API::None;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

auto Renderer::initialize(const std::unique_ptr<WindowInterface>& windowInterface) -> void {
    AdelieLogDebug("Start initializing the selected rendering API");
    switch (sAPI) {
        case API::Vulkan:
            std::make_unique<VulkanRenderer>(windowInterface);
            break;
        case API::None:
        default:
            throw RuntimeException("No rendering API selected");
    }
}