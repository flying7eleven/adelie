// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANVERTEX_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANVERTEX_HXX__

namespace adelie::renderer::vulkan {
    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <glm/glm.hpp>

    struct ADELIE_API VulkanVertex {
            glm::vec3 pos;
            glm::vec3 color;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;

            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(VulkanVertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

                // Position
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

                // Color
                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

                // Normal
                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[2].offset = offsetof(VulkanVertex, normal);

                // Texture coordinates
                attributeDescriptions[3].binding = 0;
                attributeDescriptions[3].location = 3;
                attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[3].offset = offsetof(VulkanVertex, texCoord);

                // Tangent
                attributeDescriptions[4].binding = 0;
                attributeDescriptions[4].location = 4;
                attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[4].offset = offsetof(VulkanVertex, tangent);

                return attributeDescriptions;
            }
    };
} /* namespace adelie::renderer::vulkan */

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANVERTEX_HXX__) */