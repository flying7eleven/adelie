// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANSHADERMANAGER_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANSHADERMANAGER_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <string>
    #include <vector>

namespace adelie::renderer::vulkan {
    class ADELIE_API VulkanShaderManager {
        public:
            static auto readFile(const std::string& filename) -> std::vector<char>;
            static auto createShaderModule(VkDevice device, const std::vector<char>& code) -> VkShaderModule;
    }; /* class VulkanShaderManager */

}  // namespace adelie::renderer::vulkan

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANSHADERMANAGER_HXX__) */