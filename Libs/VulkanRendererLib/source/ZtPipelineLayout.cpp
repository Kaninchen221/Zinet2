#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	bool PipelineLayout::create(const Device& device) noexcept
	{
		if (isValid())
			return false;

		const VkPipelineLayoutCreateInfo pipelineLayoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

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