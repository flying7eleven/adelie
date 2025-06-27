// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/core/Assert.hxx>
#include <adelie/core/renderer/WindowFactory.hxx>
#include <adelie/exception/RuntimeException.hxx>
#include <adelie/exception/VulkanRuntimeException.hxx>
#include <adelie/io/Logger.hxx>
#include <adelie/renderer/vulkan/VulkanExtensionManager.hxx>
#include <adelie/renderer/vulkan/VulkanRenderer.hxx>

using adelie::core::renderer::WindowFactory;
using adelie::core::renderer::WindowType;
using adelie::exception::RuntimeException;
using adelie::exception::VulkanRuntimeException;
using adelie::renderer::vulkan::VulkanExtensionManager;
using adelie::renderer::vulkan::VulkanRenderer;

VulkanRenderer::VulkanRenderer(const std::unique_ptr<core::renderer::WindowInterface>& windowInterface) {
    mInstance = VK_NULL_HANDLE;
    mSurface = VK_NULL_HANDLE;
    mPhysicalDevice = VK_NULL_HANDLE;

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

#ifdef ADELIE_PLATFORM_MACOS
    AdelieLogDebug("Enabling portability enumeration for MoltenVK");
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    auto extensions = VulkanExtensionManager::getRequiredInstanceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
        throw VulkanRuntimeException("Failed to create Vulkan instance!");
    }

    createSurface(windowInterface);
    pickPhysicalDevice();
}

VulkanRenderer::~VulkanRenderer() noexcept {
    AdelieLogDebug("Cleaning up VulkanRenderer");

    if (VK_NULL_HANDLE != mPhysicalDevice) {
        // TODO: ???
        mPhysicalDevice = VK_NULL_HANDLE;
    }

    if (VK_NULL_HANDLE != mSurface) {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
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
            createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
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
    std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilies(device);

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
        const std::vector<VkPresentModeKHR> presentModes = getSurfacePresentModes(device);
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