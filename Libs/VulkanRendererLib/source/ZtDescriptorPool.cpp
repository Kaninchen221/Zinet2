#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{
	DescriptorPool::DescriptorPoolSizes DescriptorPool::GetDefaultPoolSizes() noexcept
	{
		return DescriptorPoolSizes
		{
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
	}

	VkDescriptorPoolCreateInfo DescriptorPool::GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes) noexcept
	{
		return VkDescriptorPoolCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = GetDefaultMaxSets(),
			.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size()),
			.pPoolSizes = poolSizes.data()
		};
	}

	bool DescriptorPool::create(const Device& device, const VkDescriptorPoolCreateInfo& createInfo)
	{
		const auto result = vkCreateDescriptorPool(device.get(), &createInfo, nullptr, &get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create DescriptorPool, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void DescriptorPool::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyDescriptorPool(device.get(), get(), nullptr);
			invalidateInternal();
		}
	}
}