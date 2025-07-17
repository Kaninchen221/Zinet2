#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{
	bool ShaderModule::create(const Device& device, const shaderc::SpvCompilationResult& compilationResult) ZINET_API_POST
	{
		if (isValid())
			return false;

		const auto count = compilationResult.cend() - compilationResult.cbegin();
		const size_t codeSize = count * sizeof(std::uint32_t);

		const VkShaderModuleCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.codeSize = codeSize,
			.pCode = compilationResult.cbegin()
		};

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

	VkPipelineShaderStageCreateInfo ShaderModule::createPipelineShaderStageCreateInfo(const ShaderType shaderType) const ZINET_API_POST
	{
		return VkPipelineShaderStageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.stage = ShaderTypeToVkShaderStage(shaderType),
			.module = objectHandle,
			.pName = "main",
			.pSpecializationInfo = nullptr
		};
	}

	void ShaderModule::destroy(const Device& device) ZINET_API_POST
	{
		if (isValid())
		{
			vkDestroyShaderModule(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}