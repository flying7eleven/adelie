// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <adelie/core/renderer/WindowInterface.hxx>

namespace adelie::renderer::vulkan {

    class ADELIE_API VulkanRenderer {
        public:
            VulkanRenderer(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface);

            ~VulkanRenderer() noexcept;

            VulkanRenderer(const VulkanRenderer&) = delete;

            auto operator=(VulkanRenderer const&) -> VulkanRenderer& = delete;

            VulkanRenderer(VulkanRenderer&&) = delete;

            auto operator=(VulkanRenderer&&) -> VulkanRenderer& = delete;

        private:
            static auto getQueueFamilies(VkPhysicalDevice device) -> std::vector<VkQueueFamilyProperties>;

            auto createSurface(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) -> void;
            auto getSurfaceFormats(VkPhysicalDevice device) -> std::vector<VkSurfaceFormatKHR>;
            auto getSurfacePresentModes(VkPhysicalDevice device) -> std::vector<VkPresentModeKHR>;
            auto isDeviceSurfaceSupported(VkPhysicalDevice device, uint32_t queueFamilyIndex) -> bool;
            auto findQueueFamilies(VkPhysicalDevice device) -> uint32_t;
            auto isDeviceSuitable(VkPhysicalDevice device) -> bool;
            auto pickPhysicalDevice() -> void;

            VkInstance mInstance;
            VkSurfaceKHR mSurface;
            VkPhysicalDevice mPhysicalDevice;

    }; /* class VulkanRenderer */

} /* namespace adelie::renderer::vulkan */

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__) */
