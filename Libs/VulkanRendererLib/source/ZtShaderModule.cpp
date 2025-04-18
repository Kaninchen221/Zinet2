#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{
	bool ShaderModule::create(const Device& device, const shaderc::SpvCompilationResult& compilationResult) noexcept
	{
		const auto count = compilationResult.cend() - compilationResult.cbegin();
		const size_t codeSize = count * sizeof(std::uint32_t);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = codeSize;
		createInfo.pCode = compilationResult.cbegin();

		const auto result = vkCreateShaderModule(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Can't create shader module");
			return false;
		}
	}

	VkPipelineShaderStageCreateInfo ShaderModule::createPipelineShaderStageCreateInfo(const ShaderType shaderType) const noexcept
	{
		VkPipelineShaderStageCreateInfo result{};
		result.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		result.module = get();
		result.stage = ShaderTypeToVkShaderStage(shaderType);
		result.pName = "main";

		return result;
	}

	void ShaderModule::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyShaderModule(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}