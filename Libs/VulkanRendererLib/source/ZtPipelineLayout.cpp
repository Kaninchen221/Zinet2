#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	VkPipelineLayoutCreateInfo PipelineLayout::GetDefaultCreateInfo(const std::vector<DescriptorSetLayout::VulcanType>& vkDescriptorSetLayouts) noexcept
	{
		return VkPipelineLayoutCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.setLayoutCount = static_cast<uint32_t>(vkDescriptorSetLayouts.size()),
			.pSetLayouts = vkDescriptorSetLayouts.empty() ? nullptr : vkDescriptorSetLayouts.data(),
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};
	}

	bool PipelineLayout::create(const Device& device, const VkPipelineLayoutCreateInfo& createInfo)
	{
		if (isValid())
			return false;

		const auto result = vkCreatePipelineLayout(device.get(), &createInfo, nullptr, &get());
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
			vkDestroyPipelineLayout(device.get(), get(), nullptr);
			invalidateInternal();
		}
	}

}