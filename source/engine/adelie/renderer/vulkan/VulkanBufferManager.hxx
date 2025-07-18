// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANBUFFERMANAGER_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANBUFFERMANAGER_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>

namespace adelie::renderer::vulkan {

    class ADELIE_API VulkanBufferManager {
        public:
            static auto createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
                -> void;

            static auto copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void;

            static auto findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;

            static auto beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) -> VkCommandBuffer;

            static auto endSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer) -> void;

    }; /* class VulkanBufferManager */

} /* namespace adelie::renderer::vulkan */

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANBUFFERMANAGER_HXX__) */
