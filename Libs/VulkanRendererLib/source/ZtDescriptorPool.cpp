#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	VkDescriptorPoolSize DescriptorPool::GetDefaultDescriptorPoolSize() noexcept
	{
		return VkDescriptorPoolSize
		{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1
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
		const auto result = vkCreateDescriptorPool(device.get(), &createInfo, nullptr, &objectHandle);
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
			vkDestroyDescriptorPool(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}