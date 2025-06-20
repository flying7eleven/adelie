// Copyright (c) 2025 by Tim Janke. All rights reserved.

#if !defined(__ADELIE_EXCEPTION_VULKANRUNTIMEEXCEPTION_HXX__)
    #define __ADELIE_EXCEPTION_VULKANRUNTIMEEXCEPTION_HXX__

    #include <vulkan/vulkan.h>

    #include <adelie/adelie.hxx>
    #include <adelie/exception/RuntimeException.hxx>
    #include <string>

namespace adelie::exception {

    class ADELIE_API VulkanRuntimeException : public RuntimeException {
        public:
            explicit VulkanRuntimeException(const std::string& message) noexcept;

            VulkanRuntimeException(const std::string& message, const VkResult& returnCode) noexcept;

            ~VulkanRuntimeException() noexcept override;

            [[nodiscard]] auto getVulkanErrorCode() const -> std::string;

        private:
            VkResult mErrorResultCode;

    }; /* class VulkanRuntimeException */

} /* namespace adelie::exception */

#endif /* if !defined(__ADELIE_EXCEPTION_VULKANRUNTIMEEXCEPTION_HXX__) */
