#include "Zinet/VulkanRenderer/ZtDescriptorSet.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::vulkan_renderer
{
	VkDescriptorSetAllocateInfo DescriptorSet::GetDefaultAllocateInfo(
		const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& descriptorSetLayouts) noexcept
	{
		return VkDescriptorSetAllocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = descriptorPool.get(),
			.descriptorSetCount = static_cast<std::uint32_t>(descriptorSetLayouts.size()),
			.pSetLayouts = descriptorSetLayouts.data()
		};
	}

	bool DescriptorSet::create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo) noexcept
	{
		const auto result = vkAllocateDescriptorSets(device.get(), &allocateInfo, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create DescriptorSet, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	VkWriteDescriptorSet DescriptorSet::GetDefaultWriteDescriptorSet() noexcept
	{
		return VkWriteDescriptorSet
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = nullptr,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImageInfo = nullptr,
			.pBufferInfo = nullptr,
			.pTexelBufferView = nullptr
		};
	}

	VkDescriptorBufferInfo DescriptorSet::GetBufferInfo(const Buffer& buffer) noexcept
	{
		return VkDescriptorBufferInfo
		{
			.buffer = buffer.get(),
			.offset = 0,
			.range = buffer.getSize()
		};
	}

	void DescriptorSet::update(const Device& device, const VkWriteDescriptorSet& writeDescriptorSet) const noexcept
	{
		vkUpdateDescriptorSets(device.get(), 1, &writeDescriptorSet, 0, nullptr);
	}

}