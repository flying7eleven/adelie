// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/exception/VulkanRuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanExtensionManager.hxx>
#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <set>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::exception::VulkanRuntimeException;
using adelie::renderer::vulkan::VulkanExtensionManager;

auto VulkanExtensionManager::getRequiredInstanceExtensions() -> std::vector<std::string> {
    std::vector<std::string> requestedExtensions;
#if defined(ADELIE_BUILD_TYPE_DEBUG)
    requestedExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

#ifdef ADELIE_PLATFORM_WINDOWS
    requestedExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

#ifdef ADELIE_PLATFORM_LINUX
    switch (WindowFactory::getWindowType()) {
        case WindowType::WAYLAND_API:
            requestedExtensions.emplace_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
            break;
        case WindowType::XCB_API:
            requestedExtensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            break;
        default:
            throw RuntimeException("Unknown Window type!");
    }
#endif

#ifdef ADELIE_PLATFORM_MACOS
    requestedExtensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
    requestedExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    requestedExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif

    requestedExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

    std::vector<std::string> selectedExtensions;

    uint32_t extensionCount;
    if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to enumerate supported Vulkan instance extension count", result);
    }

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to enumerate supported Vulkan instance extensions", result);
    }
    AdelieLogDebug("Found {} supported Vulkan extensions", availableExtensions.size());
    for (const auto& extension : availableExtensions) {
        const std::string extensionName(extension.extensionName);
        AdelieLogDebug("  {} (specification version {})", extensionName, extension.specVersion);

        if (const auto it = std::ranges::find_if(requestedExtensions, [&extensionName](const auto& requested) { return extensionName == requested; }); it != requestedExtensions.end()) {
            selectedExtensions.push_back(extensionName);
        }
    }

    if (!selectedExtensions.empty()) {
        AdelieLogDebug("Requesting the following Vulkan instance extension(s): {}", boost::algorithm::join(selectedExtensions, ", "));
    }

    return selectedExtensions;
}

auto VulkanExtensionManager::getRequiredInstanceLayers() -> std::vector<std::string> {
    std::vector<std::string> requestedLayers;
#if defined(ADELIE_BUILD_TYPE_DEBUG)
    requestedLayers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

    std::vector<std::string> selectedLayers;

    uint32_t layerCount;
    if (const auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to enumerate supported Vulkan layer count", result);
    }

    std::vector<VkLayerProperties> availableLayers(layerCount);
    if (const auto result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to enumerate supported Vulkan layers", result);
    }

    AdelieLogDebug("Found {} supported Vulkan layers", availableLayers.size());
    for (const auto& layer : availableLayers) {
        const std::string layerName(layer.layerName);
        const std::string layerDescription(layer.description);
        AdelieLogDebug("  {} ({})", layerName, layerDescription);

        if (const auto it = std::ranges::find_if(requestedLayers, [&layerName](const auto& requested) { return layerName == requested; }); it != requestedLayers.end()) {
            selectedLayers.push_back(layerName);
        }
    }

    if (!selectedLayers.empty()) {
        AdelieLogDebug("Requesting the following Vulkan instance layer(s): {}", boost::algorithm::join(selectedLayers, ", "));
    }

    return selectedLayers;
}

std::vector<const char*> VulkanExtensionManager::getRequiredDeviceExtensions() {
    std::vector extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef ADELIE_PLATFORM_MACOS
    extensions.push_back("VK_KHR_portability_subset");
#endif

    return extensions;
}

std::vector<const char*> VulkanExtensionManager::getEnabledExtensions(const std::vector<VkExtensionProperties>& availableExtensions,
                                                                      const std::vector<const char*>& requiredExtensions,
                                                                      const std::string& extensionType) {
    std::set<std::string> available;
    for (const auto& extension : availableExtensions) {
        available.insert(extension.extensionName);
    }

    std::vector<const char*> enabled;
    for (const char* required : requiredExtensions) {
        if (available.find(required) != available.end()) {
            enabled.push_back(required);
        } else {
            throw RuntimeException("Required " + extensionType + " extension not available: " + required);
        }
    }

    return enabled;
}

bool VulkanExtensionManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    auto requiredExtensions = getRequiredDeviceExtensions();
    std::set<std::string> requiredExtensionSet(requiredExtensions.begin(), requiredExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensionSet.erase(extension.extensionName);
    }

    return requiredExtensionSet.empty();
}