#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

namespace zt::vulkan_renderer
{
	VkDescriptorSetAllocateInfo DescriptorSets::GetDefaultAllocateInfo(
		const DescriptorPool& descriptorPool, const VkDescriptorSetLayouts& vkDescriptorSetLayouts) noexcept
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

	bool DescriptorSets::create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo)
	{
		handles.resize(allocateInfo.descriptorSetCount, nullptr);
		const auto result = vkAllocateDescriptorSets(device.get(), &allocateInfo, handles.data());
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

	void DescriptorSets::invalidate() noexcept
	{
		handles = { nullptr };
	}

	bool DescriptorSets::isValid() const noexcept
	{
		for (auto handle : handles)
		{
			if (!handle)
				return false;
		}

		return true;
	}

	void DescriptorSets::update(const Device& device, const DescriptorSetsUpdateData& updateData) const noexcept
	{
		auto& writeDescriptorSets = updateData.writeDescriptorSets;

		vkUpdateDescriptorSets(device.get(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}

}