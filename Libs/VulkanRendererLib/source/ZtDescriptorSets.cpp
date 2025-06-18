#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace zt::vulkan_renderer
{
	VkDescriptorSetAllocateInfo DescriptorSets::GetDefaultAllocateInfo(
		const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts) noexcept
	{
		return VkDescriptorSetAllocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = descriptorPool.get(),
			.descriptorSetCount = static_cast<std::uint32_t>(vkDescriptorSetLayouts.size()),
			.pSetLayouts = vkDescriptorSetLayouts.data()
		};
	}

	bool DescriptorSets::create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo) noexcept
	{
		const auto result = vkAllocateDescriptorSets(device.get(), &allocateInfo, &objectHandle);
		if (result == VK_SUCCESS)
		{
			count = allocateInfo.descriptorSetCount;
			return true;
		}
		else
		{
			Logger->error("Couldn't create DescriptorSets, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	VkWriteDescriptorSet DescriptorSets::GetDefaultWriteDescriptorSet() noexcept
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

	VkDescriptorBufferInfo DescriptorSets::GetBufferInfo(const Buffer& buffer) noexcept
	{
		return
		{
			.buffer = buffer.get(),
			.offset = 0,
			.range = buffer.getSize()
		};
	}

	VkDescriptorImageInfo DescriptorSets::GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept
	{
		return
		{
			.sampler = sampler.get(),
			.imageView = imageView.get(),
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // TODO: Refactor, image view or/and image should remember layout
		};
	}

	void DescriptorSets::update(const Device& device, const std::vector<VkWriteDescriptorSet>& writeDescriptorSets) const noexcept
	{
		vkUpdateDescriptorSets(device.get(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}

}