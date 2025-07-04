// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANEXTENSIONMANAGER_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANEXTENSIONMANAGER_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <vector>

namespace adelie::renderer::vulkan {

    class ADELIE_API VulkanExtensionManager {
        public:
            static auto getRequiredInstanceLayers() -> std::vector<std::string>;
            static auto getRequiredInstanceExtensions() -> std::vector<std::string>;
            static std::vector<const char*> getRequiredDeviceExtensions();
            static std::vector<const char*> getEnabledExtensions(const std::vector<VkExtensionProperties>& availableExtensions, const std::vector<const char*>& requiredExtensions, const std::string& extensionType);
            static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    }; /* class VulkanExtensionManager */

} /* namespace adelie::renderer::vulkan */

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANEXTENSIONMANAGER_HXX__) */
