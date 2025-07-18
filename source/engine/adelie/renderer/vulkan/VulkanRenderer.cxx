// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <vulkan/vk_enum_string_helper.h>

#include <adelie/adelie.hxx>
#include <adelie/core/Assert.hxx>
#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/exception/VulkanRuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanBufferManager.hxx>
#include <adelie/renderer/vulkan/VulkanExtensionManager.hxx>
#include <adelie/renderer/vulkan/VulkanRenderer.hxx>
#include <adelie/renderer/vulkan/VulkanShaderManager.hxx>
#include <adelie/renderer/vulkan/VulkanVertex.hxx>
#include <boost/algorithm/string/join.hpp>
#include <glm/gtc/matrix_transform.hpp>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowInterface;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::exception::VulkanRuntimeException;
using adelie::renderer::vulkan::VulkanBufferManager;
using adelie::renderer::vulkan::VulkanExtensionManager;
using adelie::renderer::vulkan::VulkanRenderer;
using adelie::renderer::vulkan::VulkanShaderManager;
using adelie::renderer::vulkan::VulkanVertex;

const std::vector<VulkanVertex> vertices = {
    // Front face
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    // Back face
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
    // Left face
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    // Right face
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    // Top face
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    // Bottom face
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
    // Front face (Z+)
    0, 1, 2, 2, 3, 0,
    // Back face (Z-)
    4, 5, 6, 6, 7, 4,
    // Left face (X-)
    8, 9, 10, 10, 11, 8,
    // Right face (X+)
    12, 13, 14, 14, 15, 12,
    // Top face (Y+)
    16, 17, 18, 18, 19, 16,
    // Bottom face (Y-)
    20, 21, 22, 22, 23, 20};

struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
};

VulkanRenderer::VulkanRenderer(const std::shared_ptr<WindowInterface>& windowInterface) {
    mInstance = VK_NULL_HANDLE;
    mDebugMessenger = VK_NULL_HANDLE;
    mSurface = VK_NULL_HANDLE;
    mPhysicalDevice = VK_NULL_HANDLE;
    mLogicalDevice = VK_NULL_HANDLE;
    mSelectedGraphicsQueue = VK_NULL_HANDLE;
    mSwapChain = VK_NULL_HANDLE;
    mSwapChainImages.clear();
    mSwapChainImageFormat = VK_FORMAT_UNDEFINED;
    mSwapChainExtent = {.width = 0, .height = 0};
    mSwapChainImageViews.clear();
    mRenderPass = VK_NULL_HANDLE;
    mDescriptorSetLayout = VK_NULL_HANDLE;
    mPipelineLayout = VK_NULL_HANDLE;
    mGraphicsPipeline = VK_NULL_HANDLE;
    mSwapChainFramebuffers.clear();
    mCommandPool = VK_NULL_HANDLE;

    mVertexBuffer = VK_NULL_HANDLE;
    mVertexBufferMemory = VK_NULL_HANDLE;
    mIndexBuffer = VK_NULL_HANDLE;
    mIndexBufferMemory = VK_NULL_HANDLE;
    mUniformBuffers.clear();
    mUniformBuffersMemory.clear();

    mTextureImage = VK_NULL_HANDLE;
    mTextureImageMemory = VK_NULL_HANDLE;
    mTextureImageView = VK_NULL_HANDLE;
    mTextureSampler = VK_NULL_HANDLE;
    mNormalMapImage = VK_NULL_HANDLE;
    mNormalMapImageMemory = VK_NULL_HANDLE;
    mNormalMapImageView = VK_NULL_HANDLE;
    mRoughnessMapImage = VK_NULL_HANDLE;
    mRoughnessMapImageMemory = VK_NULL_HANDLE;
    mRoughnessMapImageView = VK_NULL_HANDLE;
    mImageAvailableSemaphores.clear();
    mRenderFinishedSemaphores.clear();
    mInFlightFences.clear();
    mCommandBuffers.clear();
    mCurrentFrame = 0;
    mDescriptorSets.clear();
    mDescriptorPool = VK_NULL_HANDLE;
    mWindowInterface = windowInterface;

    AdelieLogDebug("Start initializing VulkanRenderer");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Adelie Application";        // TODO: make that configurable
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // TODO: use the application version
    appInfo.pEngineName = "Adelie Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);  // TODO: use the correct version
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto selectedInstanceLayers = VulkanExtensionManager::getRequiredInstanceLayers();
    auto selectedInstanceExtensions = VulkanExtensionManager::getRequiredInstanceExtensions();

    std::vector<const char*> instanceLayers;
    instanceLayers.reserve(selectedInstanceLayers.size());
    for (const auto& layer : selectedInstanceLayers) {
        instanceLayers.push_back(layer.c_str());
    }

    std::vector<const char*> instanceExtensions;
    instanceExtensions.reserve(selectedInstanceExtensions.size());
    for (const auto& extension : selectedInstanceExtensions) {
        instanceExtensions.emplace_back(extension.c_str());
    }

    createInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());
    createInfo.ppEnabledLayerNames = instanceLayers.data();

#if defined(ADELIE_PLATFORM_MACOS)
    AdelieLogDebug("Enabling portability enumeration for MoltenVK");
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    const auto extensions = VulkanExtensionManager::getRequiredInstanceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    std::vector<std::string> extensionNames;
    extensionNames.reserve(extensions.size());
    for (const auto& extension : extensions) {
        extensionNames.emplace_back(extension);
    }
    AdelieLogDebug("Requesting the following Vulkan extension(s): {}", boost::algorithm::join(extensionNames, ", "));

    if (const auto result = vkCreateInstance(&createInfo, nullptr, &mInstance); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create Vulkan instance!", result);
    }

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
    debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsCreateInfo.pfnUserCallback = debugCallback;
    debugUtilsCreateInfo.pUserData = nullptr;

    if (const auto createDebugUtilsResult = createDebugUtilsMessengerEXT(&debugUtilsCreateInfo, nullptr, &mDebugMessenger); createDebugUtilsResult != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to set up debug messenger", createDebugUtilsResult);
    }

    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();

    /* specific for the test only: START */
    calculateTangents(const_cast<std::vector<VulkanVertex>&>(vertices), indices);

    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();

    createTexture("albedo.png", VK_FORMAT_R8G8B8A8_SRGB, mTextureImage, mTextureImageMemory, mTextureImageView);
    createTexture("normal.png", VK_FORMAT_R8G8B8A8_UNORM, mNormalMapImage, mNormalMapImageMemory, mNormalMapImageView);              // Assuming UNORM for normal map
    createTexture("roughness.png", VK_FORMAT_R8G8B8A8_UNORM, mRoughnessMapImage, mRoughnessMapImageMemory, mRoughnessMapImageView);  // Assuming UNORM

    createTextureSampler();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();

    mainLoop();
    /* specific for the test only: END */
}

VulkanRenderer::~VulkanRenderer() noexcept {
    AdelieLogDebug("Cleaning up VulkanRenderer");
    vkDeviceWaitIdle(mLogicalDevice);

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        vkDestroySemaphore(mLogicalDevice, mRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(mLogicalDevice, mImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(mLogicalDevice, mInFlightFences[i], nullptr);
    }

    if (VK_NULL_HANDLE != mCommandPool) {
        vkDestroyCommandPool(mLogicalDevice, mCommandPool, nullptr);
        mCommandPool = VK_NULL_HANDLE;
        AdelieLogTrace("  command pool destroyed");
    }

    if (!mSwapChainFramebuffers.empty()) {
        for (const auto frameBuffer : mSwapChainFramebuffers) {
            vkDestroyFramebuffer(mLogicalDevice, frameBuffer, nullptr);
        }
        mSwapChainFramebuffers.clear();
        AdelieLogTrace("  frame buffers destroyed");
    }

    if (VK_NULL_HANDLE != mPipelineLayout) {
        vkDestroyPipelineLayout(mLogicalDevice, mPipelineLayout, nullptr);
        mPipelineLayout = VK_NULL_HANDLE;
        AdelieLogTrace("  pipeline layout destroyed");
    }

    if (VK_NULL_HANDLE != mGraphicsPipeline) {
        vkDestroyPipeline(mLogicalDevice, mGraphicsPipeline, nullptr);
        mGraphicsPipeline = VK_NULL_HANDLE;
        AdelieLogTrace("  graphics pipeline destroyed");
    }

    if (VK_NULL_HANDLE != mDescriptorSetLayout) {
        vkDestroyDescriptorSetLayout(mLogicalDevice, mDescriptorSetLayout, nullptr);
        mDescriptorSetLayout = VK_NULL_HANDLE;
        AdelieLogTrace("  descriptor set layout destroyed");
    }

    if (!mSwapChainImageViews.empty()) {
        for (const auto view : mSwapChainImageViews) {
            vkDestroyImageView(mLogicalDevice, view, nullptr);
        }
        mSwapChainImageViews.clear();
        AdelieLogTrace("  swap chain image views destroyed");
    }

    if (!mSwapChainImages.empty()) {
        for (const auto image : mSwapChainImages) {
            vkDestroyImage(mLogicalDevice, image, nullptr);
        }
        mSwapChainImages.clear();
        AdelieLogTrace("  swap chain images destroyed");
    }

    if (VK_NULL_HANDLE != mRenderPass) {
        vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr);
        mRenderPass = VK_NULL_HANDLE;
        AdelieLogTrace("  render pass destroyed");
    }

    if (VK_NULL_HANDLE != mSwapChain) {
        vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);
        mSwapChain = VK_NULL_HANDLE;
        AdelieLogTrace("  swap chain destroyed");
    }

    if (VK_NULL_HANDLE != mSelectedGraphicsQueue) {
        // TODO: ???
        mSelectedGraphicsQueue = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mLogicalDevice) {
        vkDestroyDevice(mLogicalDevice, nullptr);
        mLogicalDevice = VK_NULL_HANDLE;
        AdelieLogTrace("  logical device freed");
    }

    if (VK_NULL_HANDLE != mPhysicalDevice) {
        // TODO: ???
        mPhysicalDevice = VK_NULL_HANDLE;
        AdelieLogTrace("  physical device freed");
    }

    if (VK_NULL_HANDLE != mSurface) {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
        AdelieLogTrace("  surface handle destroyed");
    }

    if (VK_NULL_HANDLE != mDebugMessenger) {
        destroyDebugUtilsMessengerEXT(nullptr);
        mDebugMessenger = VK_NULL_HANDLE;
        AdelieLogTrace("  debug messenger destroyed");
    }

    if (VK_NULL_HANDLE != mInstance) {
        vkDestroyInstance(mInstance, nullptr);
        AdelieLogTrace("  Vulkan instance destroyed");
        mInstance = nullptr;
    }
}

auto VulkanRenderer::createIndexBuffer() -> void {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                                      stagingBufferMemory);

    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBuffer), "createIndexBuffer.stagingBuffer", VK_OBJECT_TYPE_BUFFER);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBufferMemory), "createIndexBuffer.stagingBufferMemory", VK_OBJECT_TYPE_DEVICE_MEMORY);

    void* data;
    vkMapMemory(mLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(mLogicalDevice, stagingBufferMemory);

    VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mIndexBuffer,
                                      mIndexBufferMemory);

    debugUtilsObjectName(reinterpret_cast<uint64_t>(mIndexBuffer), "createIndexBuffer.mIndexBuffer", VK_OBJECT_TYPE_BUFFER);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(mIndexBufferMemory), "createIndexBuffer.mIndexBufferMemory", VK_OBJECT_TYPE_DEVICE_MEMORY);

    VulkanBufferManager::copyBuffer(mLogicalDevice, mCommandPool, mSelectedGraphicsQueue, stagingBuffer, mIndexBuffer, bufferSize);

    vkDestroyBuffer(mLogicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(mLogicalDevice, stagingBufferMemory, nullptr);
}

auto VulkanRenderer::createUniformBuffers() -> void {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    mUniformBuffers.resize(mSwapChainImages.size());
    mUniformBuffersMemory.resize(mSwapChainImages.size());

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                          mUniformBuffers[i], mUniformBuffersMemory[i]);

        debugUtilsObjectName(reinterpret_cast<uint64_t>(mUniformBuffers[i]), std::format("createUniformBuffers.mUniformBuffers[{}]", i).c_str(), VK_OBJECT_TYPE_BUFFER);
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mUniformBuffersMemory[i]), std::format("createUniformBuffers.mUniformBuffersMemory[{}]", i).c_str(), VK_OBJECT_TYPE_DEVICE_MEMORY);
    }
}

auto VulkanRenderer::createVertexBuffer() -> void {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                                      stagingBufferMemory);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBuffer), "createVertexBuffer.stagingBuffer", VK_OBJECT_TYPE_BUFFER);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBufferMemory), "createVertexBuffer.stagingBufferMemory.", VK_OBJECT_TYPE_DEVICE_MEMORY);

    void* data;
    vkMapMemory(mLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(mLogicalDevice, stagingBufferMemory);

    VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVertexBuffer,
                                      mVertexBufferMemory);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(mVertexBuffer), "createVertexBuffer.mVertexBuffer", VK_OBJECT_TYPE_BUFFER);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(mVertexBufferMemory), "createVertexBuffer.mVertexBufferMemory", VK_OBJECT_TYPE_DEVICE_MEMORY);

    VulkanBufferManager::copyBuffer(mLogicalDevice, mCommandPool, mSelectedGraphicsQueue, stagingBuffer, mVertexBuffer, bufferSize);

    vkDestroyBuffer(mLogicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(mLogicalDevice, stagingBufferMemory, nullptr);
}

auto VulkanRenderer::calculateTangents(std::vector<VulkanVertex>& vertices, const std::vector<uint16_t>& indices) -> void {
    std::vector tangents(vertices.size(), glm::vec3(0.0f));
    std::vector bitangents(vertices.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) {
        VulkanVertex& v0 = vertices[indices[i + 0]];
        VulkanVertex& v1 = vertices[indices[i + 1]];
        VulkanVertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.pos - v0.pos;
        glm::vec3 edge2 = v2.pos - v0.pos;

        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        tangents[indices[i + 0]] += tangent;
        tangents[indices[i + 1]] += tangent;
        tangents[indices[i + 2]] += tangent;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        const glm::vec3& n = vertices[i].normal;
        const glm::vec3& t = tangents[i];

        vertices[i].tangent = glm::normalize(t - n * glm::dot(n, t));
    }
}

void VulkanRenderer::createSurface() {
#if defined(ADELIE_PLATFORM_MACOS)
    VkMacOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pView = mWindowInterface->getNativeWindowHandle();
    if (const auto createSurfaceResult = vkCreateMacOSSurfaceMVK(mInstance, &createInfo, nullptr, &mSurface); createSurfaceResult != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create macOS MoltenVK window surface", createSurfaceResult);
    }
#elif defined(ADELIE_PLATFORM_WINDOWS)
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = reinterpret_cast<HWND>(mWindowInterface->getNativeWindowHandle());
    createInfo.hinstance = reinterpret_cast<HINSTANCE>(mWindowInterface->getNativeDisplayHandle());
    if (vkCreateWin32SurfaceKHR(mInstance, &createInfo, nullptr, &mSurface) != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create Win32 window surface!");
    }
#elif defined(ADELIE_PLATFORM_LINUX)
    switch (WindowFactory::getWindowType()) {
        case WindowType::XCB_API: {
            VkXcbSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
            createInfo.connection = static_cast<xcb_connection_t*>(mWindowInterface->getNativeDisplayHandle());
            createInfo.window = static_cast<xcb_window_t>(reinterpret_cast<uintptr_t>(mWindowInterface->getNativeWindowHandle()));
            if (vkCreateXcbSurfaceKHR(mInstance, &createInfo, nullptr, &mSurface) != VK_SUCCESS) {
                throw VulkanRuntimeException("Failed to create XCB window surface!");
            }
        } break;
        case WindowType::WAYLAND_API: {
            VkWaylandSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
            createInfo.display = static_cast<wl_display*>(mWindowInterface->getNativeDisplayHandle());
            createInfo.surface = static_cast<wl_surface*>(mWindowInterface->getNativeWindowHandle());
            if (const auto createSurfaceResult = vkCreateWaylandSurfaceKHR(mInstance, &createInfo, nullptr, &mSurface); createSurfaceResult != VK_SUCCESS) {
                throw VulkanRuntimeException("Failed to create Wayland window surface!", createSurfaceResult);
            }
        } break;
        default:
            throw RuntimeException("Unknown Window type!");
    }

#else
    #error "Platform not supported"
#endif
}

auto VulkanRenderer::getSurfaceFormats(VkPhysicalDevice device) const -> std::vector<VkSurfaceFormatKHR> {
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, formats.data());
    return formats;
}

auto VulkanRenderer::getSurfacePresentModes(VkPhysicalDevice device) const -> std::vector<VkPresentModeKHR> {
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, presentModes.data());
    return presentModes;
}

auto VulkanRenderer::isDeviceSurfaceSupported(VkPhysicalDevice device, uint32_t queueFamilyIndex) const -> bool {
    AdelieAssert(device != VK_NULL_HANDLE, "Device must not be null");
    AdelieAssert(mSurface != VK_NULL_HANDLE, "Surface must not be null");

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamilyIndex, mSurface, &presentSupport);
    return presentSupport == VK_TRUE;
}

auto VulkanRenderer::getQueueFamilies(VkPhysicalDevice device) -> std::vector<VkQueueFamilyProperties> {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    return queueFamilies;
}

auto VulkanRenderer::queueFamilyFlagsToString(const VkQueueFlags& flags) -> std::string {
    static const struct QueueFlagMapping {
            VkQueueFlags flag;
            std::string_view name;
    } mappings[] = {{VK_QUEUE_GRAPHICS_BIT, "Graphics"},
                    {VK_QUEUE_COMPUTE_BIT, "Compute"},
                    {VK_QUEUE_TRANSFER_BIT, "Transfer"},
                    {VK_QUEUE_SPARSE_BINDING_BIT, "Sparse"},
                    {VK_QUEUE_PROTECTED_BIT, "Protected"},
                    {VK_QUEUE_VIDEO_DECODE_BIT_KHR, "Video decode"},
                    {VK_QUEUE_VIDEO_ENCODE_BIT_KHR, "Video encode"},
                    {VK_QUEUE_OPTICAL_FLOW_BIT_NV, "Optical flow"}};

    std::string result;
    for (const auto& mapping : mappings) {
        if (flags & mapping.flag) {
            if (!result.empty()) {
                result += ", ";
            }
            result += mapping.name;
        }
    }
    return result;
}

auto VulkanRenderer::findQueueFamilies(VkPhysicalDevice device) const -> uint32_t {
    uint32_t queueFamilyIndex = UINT32_MAX;
    const std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilies(device);

    AdelieLogDebug("    Found {} queue families for device", queueFamilies.size());
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        const auto& queueFamily = queueFamilies[i];
        AdelieLogDebug("      Queue Family {}: Count: {}, Flags: {}", i, queueFamily.queueCount, queueFamilyFlagsToString(queueFamily.queueFlags));
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if (isDeviceSurfaceSupported(device, i)) {
                queueFamilyIndex = i;
                break;
            }
        }
    }

    if (UINT32_MAX == queueFamilyIndex) {
        AdelieLogError("    No graphics queue family of the device was able to present the used surface format");
    }

    return queueFamilyIndex;
}

auto VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device) const -> bool {
    const uint32_t queueFamilyIndex = findQueueFamilies(device);
    const bool extensionsSupported = VulkanExtensionManager::checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &capabilities);
        const std::vector<VkSurfaceFormatKHR> formats = getSurfaceFormats(device);
#if defined(ADELIE_BUILD_TYPE_DEBUG)
        AdelieLogDebug("    Supported surface formats");
        for (const auto& [format, _] : formats) {
            AdelieLogDebug("      {}", string_VkFormat(format));
        }
#endif
        const std::vector<VkPresentModeKHR> presentModes = getSurfacePresentModes(device);
#if defined(ADELIE_BUILD_TYPE_DEBUG)
        AdelieLogDebug("    Supported present modes");
        for (const auto& presentMode : presentModes) {
            AdelieLogDebug("      {}", string_VkPresentModeKHR(presentMode));
        }
#endif
        swapChainAdequate = !formats.empty() && !presentModes.empty();
    }

    return queueFamilyIndex != UINT32_MAX && extensionsSupported && swapChainAdequate;
}

auto VulkanRenderer::pickPhysicalDevice() -> void {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);
    AdelieLogDebug("Found {} GPU(s) with Vulkan support", deviceCount);
    if (deviceCount == 0) {
        throw VulkanRuntimeException("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        AdelieLogDebug("  Checking device: {}", deviceProperties.deviceName);
        if (isDeviceSuitable(device)) {
            mPhysicalDevice = device;
            break;
        }
    }

    if (VK_NULL_HANDLE == mPhysicalDevice) {
        throw VulkanRuntimeException("Could not find any rendering device that supports the required properties");
    }
}

auto VulkanRenderer::createLogicalDevice() -> void {
    const auto queueFamilyIndex = findQueueFamilies(mPhysicalDevice);

    constexpr float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    const auto deviceExtensions = VulkanExtensionManager::getRequiredDeviceExtensions();

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (const auto createDeviceResult = vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice); createDeviceResult != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create logical device!", createDeviceResult);
    }

    vkGetDeviceQueue(mLogicalDevice, queueFamilyIndex, 0, &mSelectedGraphicsQueue);
}

auto VulkanRenderer::createImageViews() -> void {
    mSwapChainImageViews.resize(mSwapChainImages.size());

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = mSwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = mSwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (const auto createImageViewResult = vkCreateImageView(mLogicalDevice, &createInfo, nullptr, &mSwapChainImageViews[i]); createImageViewResult != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to create image views", createImageViewResult);
        }
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mSwapChainImageViews[i]), std::format("mSwapChainImageViews[{}]", i).c_str(), VK_OBJECT_TYPE_IMAGE_VIEW);
    }
}

auto VulkanRenderer::createSwapChain() -> void {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, presentModes.data());

    const auto [format, colorSpace] = chooseSwapSurfaceFormat(formats);
    const auto presentMode = chooseSwapPresentMode(presentModes);
    const auto extent = chooseSwapExtent(capabilities);

    auto imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }
    AdelieLogDebug("Selected {} images to be in-flight based on device capabilities (min. {} / max. {})", imageCount, capabilities.minImageCount, capabilities.maxImageCount);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (const auto createSwapChainResult = vkCreateSwapchainKHR(mLogicalDevice, &createInfo, nullptr, &mSwapChain); createSwapChainResult != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create swap chain", createSwapChainResult);
    }

    vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &imageCount, nullptr);
    mSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &imageCount, mSwapChainImages.data());
    for (auto i = 0; i < mSwapChainImages.size(); i++) {
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mSwapChainImages[i]), std::format("mSwapChainImages[{}]", i).c_str(), VK_OBJECT_TYPE_IMAGE);
    }

    mSwapChainImageFormat = format;
    mSwapChainExtent = extent;
}

auto VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) -> VkSurfaceFormatKHR {
    for (const auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return formats[0];
}

auto VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) -> VkPresentModeKHR {
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

auto VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) -> VkExtent2D {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int width, height;
    mWindowInterface->getWindowSize(width, height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

auto VulkanRenderer::debugUtilsObjectName(const uint64_t& objectHandle, const char* objectName, const VkObjectType& objectType) -> void {
    const auto debugUtilsObjectNameInfo = VkDebugUtilsObjectNameInfoEXT{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = nullptr,
        .objectType = objectType,
        .objectHandle = objectHandle,
        .pObjectName = objectName,
    };
    if (const auto result = setDebugUtilsObjectNameEXT(&debugUtilsObjectNameInfo); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to set the debug object name for an object", result);
    }
}

auto VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                              VkDebugUtilsMessageTypeFlagsEXT messageType,
                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                              void* /*pUserData*/) -> VKAPI_ATTR VkBool32 {
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        AdelieLogTrace("Validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;  // we can continue, it's not severe enough to terminate
    }

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        AdelieLogInformation("Validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;  // we can continue, it's not severe enough to terminate
    }

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        AdelieLogWarning("Validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;  // we can continue, it's not severe enough to terminate
    }

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            AdelieLogFatal("Vulkan API usage validation failed: {}", pCallbackData->pMessageIdName);
            if (pCallbackData->objectCount > 0) {
                AdelieLogFatal("  Causing objects: {}", pCallbackData->objectCount);
                for (auto i = 0u; i < pCallbackData->objectCount; i++) {
                    AdelieLogFatal("    Type: {}", string_VkObjectType(pCallbackData->pObjects[i].objectType));
                    AdelieLogFatal("    Handle: 0x{:x}", pCallbackData->pObjects[i].objectHandle);
                    if (nullptr != pCallbackData->pObjects[i].pObjectName) {
                        AdelieLogFatal("    Name: {}", std::string(pCallbackData->pObjects[i].pObjectName));
                    }
                }
            }
            if (pCallbackData->cmdBufLabelCount > 0) {
                AdelieLogFatal("  Causing command buffer labels: {}", pCallbackData->cmdBufLabelCount);
                for (auto i = 0u; i < pCallbackData->cmdBufLabelCount; i++) {
                    AdelieLogFatal("    Name: {}", std::string(pCallbackData->pCmdBufLabels[i].pLabelName));
                }
            }
        }
        return VK_TRUE;  // it's serious enough to terminate now, this might be a severe bug
    }

    return VK_FALSE;  // we can continue, it's not severe enough to terminate
}

auto VulkanRenderer::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const -> VkResult {
    if (const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT")); func != nullptr) {
        return func(mInstance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

auto VulkanRenderer::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) const -> void {
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT")); func != nullptr) {
        func(mInstance, mDebugMessenger, pAllocator);
    }
}

auto VulkanRenderer::setDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* nameInfo) const -> VkResult {
    if (const auto func = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(mInstance, "vkSetDebugUtilsObjectNameEXT")); func != nullptr) {
        return func(mLogicalDevice, nameInfo);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

auto VulkanRenderer::createRenderPass() -> void {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = mSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (const auto result = vkCreateRenderPass(mLogicalDevice, &renderPassInfo, nullptr, &mRenderPass); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create render pass", result);
    }
}

auto VulkanRenderer::createDescriptorSetLayout() -> void {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding baseColorSamplerLayoutBinding{};
    baseColorSamplerLayoutBinding.binding = 1;
    baseColorSamplerLayoutBinding.descriptorCount = 1;
    baseColorSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    baseColorSamplerLayoutBinding.pImmutableSamplers = nullptr;
    baseColorSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding normalSamplerLayoutBinding{};
    normalSamplerLayoutBinding.binding = 2;
    normalSamplerLayoutBinding.descriptorCount = 1;
    normalSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normalSamplerLayoutBinding.pImmutableSamplers = nullptr;
    normalSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding roughnessSamplerLayoutBinding{};
    roughnessSamplerLayoutBinding.binding = 3;
    roughnessSamplerLayoutBinding.descriptorCount = 1;
    roughnessSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    roughnessSamplerLayoutBinding.pImmutableSamplers = nullptr;
    roughnessSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    const std::array bindings = {uboLayoutBinding, baseColorSamplerLayoutBinding, normalSamplerLayoutBinding, roughnessSamplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (const auto result = vkCreateDescriptorSetLayout(mLogicalDevice, &layoutInfo, nullptr, &mDescriptorSetLayout); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create descriptor set layout", result);
    }
}

auto VulkanRenderer::createGraphicsPipeline() -> void {
    auto vertShaderCode = VulkanShaderManager::readFile("shader/cube.vert.spv");
    auto fragShaderCode = VulkanShaderManager::readFile("shader/cube.frag.spv");

    auto vertShaderModule = VulkanShaderManager::createShaderModule(mLogicalDevice, vertShaderCode);
    auto fragShaderModule = VulkanShaderManager::createShaderModule(mLogicalDevice, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    auto bindingDescription = VulkanVertex::getBindingDescription();
    auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(mSwapChainExtent.width);
    viewport.height = static_cast<float>(mSwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{.offset = {0, 0}, .extent = mSwapChainExtent};

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    rasterizer.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;

    if (const auto result = vkCreatePipelineLayout(mLogicalDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create pipeline layout", result);
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = mPipelineLayout;
    pipelineInfo.renderPass = mRenderPass;
    pipelineInfo.subpass = 0;

    if (const auto result = vkCreateGraphicsPipelines(mLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mGraphicsPipeline); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create graphics pipeline", result);
    }

    vkDestroyShaderModule(mLogicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(mLogicalDevice, vertShaderModule, nullptr);
}

auto VulkanRenderer::createFramebuffers() -> void {
    mSwapChainFramebuffers.resize(mSwapChainImageViews.size());

    for (size_t i = 0; i < mSwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {mSwapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = mRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = mSwapChainExtent.width;
        framebufferInfo.height = mSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (const auto result = vkCreateFramebuffer(mLogicalDevice, &framebufferInfo, nullptr, &mSwapChainFramebuffers[i]); result != VK_SUCCESS) {
            throw VulkanRuntimeException("failed to create frame buffer", result);
        }
    }
}

auto VulkanRenderer::createCommandPool() -> void {
    const auto queueFamilyIndex = findQueueFamilies(mPhysicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (const auto result = vkCreateCommandPool(mLogicalDevice, &poolInfo, nullptr, &mCommandPool); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create command pool", result);
    }
}

auto VulkanRenderer::cleanupSwapChain() -> void {
    for (auto framebuffer : mSwapChainFramebuffers) {
        vkDestroyFramebuffer(mLogicalDevice, framebuffer, nullptr);
    }

    for (auto imageView : mSwapChainImageViews) {
        vkDestroyImageView(mLogicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        vkDestroyBuffer(mLogicalDevice, mUniformBuffers[i], nullptr);
        vkFreeMemory(mLogicalDevice, mUniformBuffersMemory[i], nullptr);
    }

    vkDestroyPipeline(mLogicalDevice, mGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(mLogicalDevice, mPipelineLayout, nullptr);
    vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr);
}

auto VulkanRenderer::recreateSwapChain() -> void {
    AdelieLogTrace("Recreating swap chain");
    int width = 0, height = 0;
    mWindowInterface->getWindowSize(width, height);

    if (width == 0 || height == 0) {
        return;
    }

    vkDeviceWaitIdle(mLogicalDevice);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
}

auto VulkanRenderer::updateUniformBuffer() -> void {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), mSwapChainExtent.width / (float)mSwapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(mLogicalDevice, mUniformBuffersMemory[mCurrentFrame], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(mLogicalDevice, mUniformBuffersMemory[mCurrentFrame]);
}

auto VulkanRenderer::mainLoop() -> void {
    while (!mWindowInterface->shouldClose()) {
        mWindowInterface->pollEvents();
        drawFrame();
    }
}

void VulkanRenderer::drawFrame() {
    vkWaitForFences(mLogicalDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(mLogicalDevice, 1, &mInFlightFences[mCurrentFrame]);

    uint32_t imageIndex;
    if (const auto result = vkAcquireNextImageKHR(mLogicalDevice, mSwapChain, UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, &imageIndex); result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw VulkanRuntimeException("Failed to acquire swap chain image", result);
    }

    if (imageIndex >= mCommandBuffers.size()) {
        throw std::runtime_error("Acquired image index out of bounds!");
    }

    updateUniformBuffer();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &mImageAvailableSemaphores[mCurrentFrame];
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &mRenderFinishedSemaphores[mCurrentFrame];

    if (const auto result = vkQueueSubmit(mSelectedGraphicsQueue, 1, &submitInfo, mInFlightFences[mCurrentFrame]); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to submit draw command buffer", result);
    }

    VkSwapchainKHR swapChains[] = {mSwapChain};
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &mRenderFinishedSemaphores[mCurrentFrame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    if (const auto result = vkQueuePresentKHR(mSelectedGraphicsQueue, &presentInfo); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to present swap chain image", result);
    }

    mCurrentFrame = (mCurrentFrame + 1) % mSwapChainImages.size();
}

auto VulkanRenderer::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) -> void {
    VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(mLogicalDevice, mCommandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  // TODO: Handle depth/stencil aspects if needed
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw VulkanRuntimeException("Unsupported layout transition");  // TODO: maybe just an assert
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    VulkanBufferManager::endSingleTimeCommands(mLogicalDevice, mCommandPool, mSelectedGraphicsQueue, commandBuffer);
}

auto VulkanRenderer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) -> void {
    VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(mLogicalDevice, mCommandPool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    VulkanBufferManager::endSingleTimeCommands(mLogicalDevice, mCommandPool, mSelectedGraphicsQueue, commandBuffer);
}

auto VulkanRenderer::createTexture(const std::string& filename, VkFormat format, VkImage& image, VkDeviceMemory& imageMemory, VkImageView& imageView) -> void {
    int texWidth, texHeight, texChannels;
    std::string fullPath = "textures/" + filename;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* pixels = stbi_load(fullPath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) {
        throw RuntimeException("Failed to load texture image: " + fullPath);
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferManager::createBuffer(mLogicalDevice, mPhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                                      stagingBufferMemory);

    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBuffer), std::format("createTexture({})", filename).c_str(), VK_OBJECT_TYPE_BUFFER);
    debugUtilsObjectName(reinterpret_cast<uint64_t>(stagingBufferMemory), std::format("createTexture({})", filename).c_str(), VK_OBJECT_TYPE_DEVICE_MEMORY);

    void* data;
    vkMapMemory(mLogicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, imageSize);
    vkUnmapMemory(mLogicalDevice, stagingBufferMemory);

    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if (const auto result = vkCreateImage(mLogicalDevice, &imageInfo, nullptr, &image); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create texture image", result);
    }
    debugUtilsObjectName(reinterpret_cast<uint64_t>(image), std::format("createTexture({})", filename).c_str(), VK_OBJECT_TYPE_IMAGE);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(mLogicalDevice, image, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VulkanBufferManager::findMemoryType(mPhysicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (const auto result = vkAllocateMemory(mLogicalDevice, &allocInfo, nullptr, &imageMemory); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to allocate texture image memory", result);
    }
    vkBindImageMemory(mLogicalDevice, image, imageMemory, 0);

    transitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    transitionImageLayout(image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(mLogicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(mLogicalDevice, stagingBufferMemory, nullptr);

    debugUtilsObjectName(reinterpret_cast<uint64_t>(imageMemory), std::format("createTexture({})", filename).c_str(), VK_OBJECT_TYPE_DEVICE_MEMORY);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (const auto result = vkCreateImageView(mLogicalDevice, &viewInfo, nullptr, &imageView); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create texture image view", result);
    }
}

auto VulkanRenderer::createDescriptorPool() -> void {
    std::array<VkDescriptorPoolSize, 4> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());
    poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[3].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(mSwapChainImages.size());

    if (const auto result = vkCreateDescriptorPool(mLogicalDevice, &poolInfo, nullptr, &mDescriptorPool); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create descriptor pool", result);
    }
}

auto VulkanRenderer::createDescriptorSets() -> void {
    std::vector layouts(mSwapChainImages.size(), mDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(mSwapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    mDescriptorSets.resize(mSwapChainImages.size());
    if (const auto result = vkAllocateDescriptorSets(mLogicalDevice, &allocInfo, mDescriptorSets.data()); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to allocate descriptor sets", result);
    }

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = mUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo baseColorImageInfo{};
        baseColorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        baseColorImageInfo.imageView = mTextureImageView;
        baseColorImageInfo.sampler = mTextureSampler;

        VkDescriptorImageInfo normalMapImageInfo{};
        normalMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        normalMapImageInfo.imageView = mNormalMapImageView;
        normalMapImageInfo.sampler = mTextureSampler;

        VkDescriptorImageInfo roughnessMapImageInfo{};
        roughnessMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        roughnessMapImageInfo.imageView = mRoughnessMapImageView;
        roughnessMapImageInfo.sampler = mTextureSampler;

        std::array<VkWriteDescriptorSet, 4> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = mDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = mDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &baseColorImageInfo;

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = mDescriptorSets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo = &normalMapImageInfo;

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = mDescriptorSets[i];
        descriptorWrites[3].dstBinding = 3;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pImageInfo = &roughnessMapImageInfo;

        vkUpdateDescriptorSets(mLogicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

auto VulkanRenderer::createCommandBuffers() -> void {
    mCommandBuffers.resize(mSwapChainImages.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());

    if (const auto result = vkAllocateCommandBuffers(mLogicalDevice, &allocInfo, mCommandBuffers.data()); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to allocate command buffers", result);
    }

    for (size_t i = 0; i < mCommandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if (const auto result = vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo); result != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to begin recording command buffer", result);
        }

        VkClearValue clearValue{};
        clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mRenderPass;
        renderPassInfo.framebuffer = mSwapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mSwapChainExtent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

        VkBuffer vertexBuffers[] = {mVertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(mCommandBuffers[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(mCommandBuffers[i], mIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptorSets[mCurrentFrame], 0, nullptr);

        vkCmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(mCommandBuffers[i]);
        if (const auto result = vkEndCommandBuffer(mCommandBuffers[i]); result != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to stop recording command buffer", result);
        }
    }
}

auto VulkanRenderer::createSyncObjects() -> void {
    mImageAvailableSemaphores.resize(mSwapChainImages.size());
    mRenderFinishedSemaphores.resize(mSwapChainImages.size());
    mInFlightFences.resize(mSwapChainImages.size());

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        if (const auto result = vkCreateSemaphore(mLogicalDevice, &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]); result != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to create synchronization object (semaphore) for available images", result);
        }
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mImageAvailableSemaphores[i]), std::format("mImageAvailableSemaphores[{}]", i).c_str(), VK_OBJECT_TYPE_SEMAPHORE);

        if (const auto result = vkCreateSemaphore(mLogicalDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]); result != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to create synchronization object (semaphore) for finished renderings", result);
        }
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mRenderFinishedSemaphores[i]), std::format("mRenderFinishedSemaphores[{}]", i).c_str(), VK_OBJECT_TYPE_SEMAPHORE);

        if (const auto result = vkCreateFence(mLogicalDevice, &fenceInfo, nullptr, &mInFlightFences[i]); result != VK_SUCCESS) {
            throw VulkanRuntimeException("Failed to create synchronization object (fence) for frames in flight", result);
        }
        debugUtilsObjectName(reinterpret_cast<uint64_t>(mInFlightFences[i]), std::format("mInFlightFences[{}]", i).c_str(), VK_OBJECT_TYPE_FENCE);
    }
}

auto VulkanRenderer::createTextureSampler() -> void {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;  // TODO: enable this feature
    // samplerInfo.maxAnisotropy = 16.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (const auto result = vkCreateSampler(mLogicalDevice, &samplerInfo, nullptr, &mTextureSampler); result != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create texture sampler", result);
    }
}