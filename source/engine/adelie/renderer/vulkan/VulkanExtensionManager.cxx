// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanExtensionManager.hxx>
#include <set>
using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::renderer::vulkan::VulkanExtensionManager;

std::vector<const char*> VulkanExtensionManager::getRequiredInstanceExtensions() {
    std::vector<const char*> extensions;

#ifdef ADELIE_PLATFORM_WINDOWS
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

#ifdef ADELIE_PLATFORM_LINUX
    switch (WindowFactory::getWindowType()) {
        case WindowType::WAYLAND_API:
            extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
            break;
        case WindowType::XCB_API:
            extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            break;
        default:
            throw RuntimeException("Unknown Window type!");
    }
#endif

#ifdef ADELIE_PLATFORM_MACOS
    extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif

    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    return extensions;
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