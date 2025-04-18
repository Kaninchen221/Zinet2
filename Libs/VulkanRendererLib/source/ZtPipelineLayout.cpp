#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	bool PipelineLayout::create(const Device& device) noexcept
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		const auto result = vkCreatePipelineLayout(device.get(), &pipelineLayoutInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create PipelineLayout, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void PipelineLayout::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyPipelineLayout(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}