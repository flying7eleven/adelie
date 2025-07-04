// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <vulkan/vk_enum_string_helper.h>

#include <adelie/core/Assert.hxx>
#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/exception/VulkanRuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanExtensionManager.hxx>
#include <adelie/renderer/vulkan/VulkanRenderer.hxx>
#include <boost/algorithm/string/join.hpp>
#include <cstring>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::exception::VulkanRuntimeException;
using adelie::renderer::vulkan::VulkanExtensionManager;
using adelie::renderer::vulkan::VulkanRenderer;

VulkanRenderer::VulkanRenderer(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) {
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

    createSurface(windowInterface);
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain(windowInterface);
}

VulkanRenderer::~VulkanRenderer() noexcept {
    AdelieLogDebug("Cleaning up VulkanRenderer");

    if (VK_NULL_HANDLE != mSwapChain) {
        vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);
        mSwapChain = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mSelectedGraphicsQueue) {
        // TODO: ???
        mSelectedGraphicsQueue = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mLogicalDevice) {
        vkDestroyDevice(mLogicalDevice, nullptr);
        mLogicalDevice = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mPhysicalDevice) {
        // TODO: ???
        mPhysicalDevice = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mSurface) {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mDebugMessenger) {
        destroyDebugUtilsMessengerEXT(mDebugMessenger, nullptr);
        mDebugMessenger = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mInstance) {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = nullptr;
    }
}

void VulkanRenderer::createSurface(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) {
#if defined(ADELIE_PLATFORM_MACOS)
    VkMacOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pView = windowInterface->getNativeWindowHandle();
    if (const auto createSurfaceResult = vkCreateMacOSSurfaceMVK(mInstance, &createInfo, nullptr, &mSurface); createSurfaceResult != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create macOS MoltenVK window surface", createSurfaceResult);
    }
#elif defined(ADELIE_PLATFORM_WINDOWS)
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = reinterpret_cast<HWND>(windowInterface->getNativeWindowHandle());
    createInfo.hinstance = reinterpret_cast<HINSTANCE>(windowInterface->getNativeDisplayHandle());
    if (vkCreateWin32SurfaceKHR(mInstance, &createInfo, nullptr, &mSurface) != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create Win32 window surface!");
    }
#elif defined(ADELIE_PLATFORM_LINUX)
    switch (WindowFactory::getWindowType()) {
        case WindowType::XCB_API: {
            VkXcbSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
            createInfo.connection = static_cast<xcb_connection_t*>(windowInterface->getNativeDisplayHandle());
            createInfo.window = static_cast<xcb_window_t>(reinterpret_cast<uintptr_t>(windowInterface->getNativeWindowHandle()));
            if (vkCreateXcbSurfaceKHR(mInstance, &createInfo, nullptr, &mSurface) != VK_SUCCESS) {
                throw VulkanRuntimeException("Failed to create XCB window surface!");
            }
        } break;
        case WindowType::WAYLAND_API: {
            VkWaylandSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
            createInfo.display = static_cast<wl_display*>(windowInterface->getNativeDisplayHandle());
            createInfo.surface = static_cast<wl_surface*>(windowInterface->getNativeWindowHandle());
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
    uint32_t queueFamilyIndex = findQueueFamilies(mPhysicalDevice);

    constexpr float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector deviceExtensions = VulkanExtensionManager::getRequiredDeviceExtensions();

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

auto VulkanRenderer::createSwapChain(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) -> void {
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

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes);
    VkExtent2D extent = chooseSwapExtent(capabilities, windowInterface);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
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

    mSwapChainImageFormat = surfaceFormat.format;
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

auto VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) -> VkExtent2D {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int width, height;
    windowInterface->getWindowSize(width, height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

auto VulkanRenderer::determineInstanceLayers() -> std::vector<std::string> {
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

auto VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                              VkDebugUtilsMessageTypeFlagsEXT messageType,
                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                              void* pUserData) -> VKAPI_ATTR VkBool32 {
    AdelieLogTrace("Validation layer: {}", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult VulkanRenderer::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    if (const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT")); func != nullptr) {
        return func(mInstance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void VulkanRenderer::destroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT")); func != nullptr) {
        func(mInstance, debugMessenger, pAllocator);
    }
}