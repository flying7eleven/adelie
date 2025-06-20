// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <adelie/exception/VulkanRuntimeException.hxx>

using adelie::exception::VulkanRuntimeException;

VulkanRuntimeException::VulkanRuntimeException(const std::string& message, const VkResult& returnCode) noexcept : RuntimeException(message) {
    mErrorResultCode = returnCode;
}

VulkanRuntimeException::VulkanRuntimeException(const std::string& message) noexcept : RuntimeException(message) {
    mErrorResultCode = VK_SUCCESS;
}

VulkanRuntimeException::~VulkanRuntimeException() noexcept = default;

auto VulkanRuntimeException::getVulkanErrorCode() const -> std::string {
    switch (mErrorResultCode) {
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        default:
            return "UNKNOWN_ERROR_CODE";
    }
}
