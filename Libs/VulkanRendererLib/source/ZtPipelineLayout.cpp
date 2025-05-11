#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	VkPipelineLayoutCreateInfo PipelineLayout::GetDefaultCreateInfo() noexcept
	{
		return VkPipelineLayoutCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};
	}

	bool PipelineLayout::create(const Device& device, const VkPipelineLayoutCreateInfo& createInfo) noexcept
	{
		if (isValid())
			return false;

		const auto result = vkCreatePipelineLayout(device.get(), &createInfo, nullptr, &objectHandle);
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