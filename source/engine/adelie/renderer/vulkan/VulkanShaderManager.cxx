#include <adelie/exception/IOException.hxx>
#include <adelie/exception/VulkanRuntimeException.hxx>
#include <adelie/renderer/vulkan/VulkanShaderManager.hxx>
#include <fstream>

using adelie::exception::IOException;
using adelie::exception::VulkanRuntimeException;
using adelie::renderer::vulkan::VulkanShaderManager;

auto VulkanShaderManager::readFile(const std::string& filename) -> std::vector<char> {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw IOException("Failed to open file: " + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

auto VulkanShaderManager::createShaderModule(VkDevice device, const std::vector<char>& code) -> VkShaderModule {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (const auto result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule); result != VK_SUCCESS) {
        throw VulkanRuntimeException("failed to create shader module", result);
    }

    return shaderModule;
}