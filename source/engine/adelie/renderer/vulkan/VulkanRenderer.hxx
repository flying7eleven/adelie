// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__)
    #define __ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <adelie/core/renderer/WindowInterface.hxx>
    #include <adelie/renderer/vulkan/VulkanVertex.hxx>

namespace adelie::renderer::vulkan {

    class ADELIE_API VulkanRenderer {
        public:
            explicit VulkanRenderer(const std::shared_ptr<core::renderer::WindowInterface>& windowInterface);

            ~VulkanRenderer() noexcept;

            VulkanRenderer(const VulkanRenderer&) = delete;

            auto operator=(VulkanRenderer const&) -> VulkanRenderer& = delete;

            VulkanRenderer(VulkanRenderer&&) = delete;

            auto operator=(VulkanRenderer&&) -> VulkanRenderer& = delete;

            auto mainLoop() -> void;

            inline auto getRenderWindow() const -> std::shared_ptr<core::renderer::WindowInterface>;

            inline auto getPhysicalDevice() const -> std::shared_ptr<VkPhysicalDevice>;

            inline auto getLogicalDevice() const -> std::shared_ptr<VkDevice>;

            inline auto getRenderSurface() const -> std::shared_ptr<VkSurfaceKHR>;

            inline auto getPipelineLayout() const -> std::shared_ptr<VkPipelineLayout>;

            inline auto getGraphicsPipeline() const -> std::shared_ptr<VkPipeline>;

            inline auto getRenderPass() const -> std::shared_ptr<VkRenderPass>;

        private:
            static auto getQueueFamilies(VkPhysicalDevice device) -> std::vector<VkQueueFamilyProperties>;

            auto createSurface() -> void;
            auto getSurfaceFormats(VkPhysicalDevice device) const -> std::vector<VkSurfaceFormatKHR>;
            auto getSurfacePresentModes(VkPhysicalDevice device) const -> std::vector<VkPresentModeKHR>;
            auto isDeviceSurfaceSupported(VkPhysicalDevice device, uint32_t queueFamilyIndex) const -> bool;
            auto findQueueFamilies(VkPhysicalDevice device) const -> uint32_t;
            auto isDeviceSuitable(VkPhysicalDevice device) const -> bool;
            auto pickPhysicalDevice() -> void;
            auto createLogicalDevice() -> void;
            auto createImageViews() -> void;
            auto createSwapChain() -> void;
            auto createRenderPass() -> void;
            auto createGraphicsPipeline() -> void;
            auto createFramebuffers() -> void;
            auto chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) -> VkExtent2D;

            static auto queueFamilyFlagsToString(const VkQueueFlags& flags) -> std::string;
            static auto chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) -> VkSurfaceFormatKHR;
            static auto chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) -> VkPresentModeKHR;

            static auto VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                 void* pUserData) -> VKAPI_ATTR VkBool32;

            auto debugUtilsObjectName(const uint64_t& objectHandle, const char* objectName, const VkObjectType& objectType) -> void;

            auto createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const -> VkResult;
            auto destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) const -> void;
            auto setDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* nameInfo) const -> VkResult;
            auto createDescriptorSetLayout() -> void;
            auto createCommandPool() -> void;

            //
            static auto calculateTangents(std::vector<VulkanVertex>& vertices, const std::vector<uint16_t>& indices) -> void;
            auto createTexture(const std::string& filename, VkFormat format, VkImage& image, VkDeviceMemory& imageMemory, VkImageView& imageView) -> void;
            auto transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) -> void;
            auto copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) -> void;
            auto createDescriptorPool() -> void;
            auto createDescriptorSets() -> void;
            auto createCommandBuffers() -> void;
            auto createSyncObjects() -> void;
            auto createTextureSampler() -> void;

            auto createVertexBuffer() -> void;
            auto createUniformBuffers() -> void;
            auto createIndexBuffer() -> void;
            auto drawFrame() -> void;
            auto recreateSwapChain() -> void;
            auto cleanupSwapChain() -> void;
            auto updateUniformBuffer() -> void;

            VkInstance mInstance;
            std::shared_ptr<VkSurfaceKHR> mSurface;
            std::shared_ptr<VkPhysicalDevice> mPhysicalDevice;
            std::shared_ptr<VkDevice> mLogicalDevice;
            std::shared_ptr<VkPipelineLayout> mPipelineLayout;
            std::shared_ptr<VkPipeline> mGraphicsPipeline;
            std::shared_ptr<VkRenderPass> mRenderPass;

            VkQueue mSelectedGraphicsQueue;
            VkSwapchainKHR mSwapChain;
            std::vector<VkImage> mSwapChainImages;
            std::vector<VkImageView> mSwapChainImageViews;
            VkFormat mSwapChainImageFormat;
            VkExtent2D mSwapChainExtent;
            VkDebugUtilsMessengerEXT mDebugMessenger;

            VkDescriptorSetLayout mDescriptorSetLayout;

            std::vector<VkFramebuffer> mSwapChainFramebuffers;
            VkCommandPool mCommandPool;

            VkBuffer mVertexBuffer;
            VkDeviceMemory mVertexBufferMemory;
            VkBuffer mIndexBuffer;
            VkDeviceMemory mIndexBufferMemory;
            std::vector<VkBuffer> mUniformBuffers;
            std::vector<VkDeviceMemory> mUniformBuffersMemory;

            VkImage mTextureImage;
            VkDeviceMemory mTextureImageMemory;
            VkImageView mTextureImageView;
            VkSampler mTextureSampler;
            VkImage mNormalMapImage;
            VkDeviceMemory mNormalMapImageMemory;
            VkImageView mNormalMapImageView;
            VkImage mRoughnessMapImage;
            VkDeviceMemory mRoughnessMapImageMemory;
            VkImageView mRoughnessMapImageView;
            std::vector<VkSemaphore> mImageAvailableSemaphores;
            std::vector<VkSemaphore> mRenderFinishedSemaphores;
            std::vector<VkFence> mInFlightFences;
            std::vector<VkCommandBuffer> mCommandBuffers;
            VkDescriptorPool mDescriptorPool;
            std::vector<VkDescriptorSet> mDescriptorSets;
            uint32_t mCurrentFrame;
            std::shared_ptr<core::renderer::WindowInterface> mWindowInterface;

    }; /* class VulkanRenderer */

} /* namespace adelie::renderer::vulkan */

#endif /* if !defined(__ADELIE_RENDERER_VULKAN_VULKANRENDERER_HXX__) */
