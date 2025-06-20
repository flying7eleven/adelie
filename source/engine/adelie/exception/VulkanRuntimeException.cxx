// Copyright (c) 2025 by Tim Janke. All rights reserved.

#include <vulkan/vk_enum_string_helper.h>

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
    return string_VkResult(mErrorResultCode);
}
