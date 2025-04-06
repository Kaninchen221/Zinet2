#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

namespace zt::vulkan_renderer
{

	VkPhysicalDeviceProperties PhysicalDevice::getVkPhysicalDeviceProperties() const noexcept
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(objectHandle, &properties);
		return properties;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getVkPhysicalDeviceFeatures() const noexcept
	{
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(objectHandle, &features);
		return features;
	}

	std::vector<VkQueueFamilyProperties> PhysicalDevice::getVkQueueFamiliesProperties() const noexcept
	{
		std::uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(objectHandle, &count, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(count);
		vkGetPhysicalDeviceQueueFamilyProperties(objectHandle, &count, queueFamilies.data());

		return queueFamilies;
	}

	std::int32_t PhysicalDevice::getQueueFamilyIndexForPresent() const noexcept
	{
		const auto familiesProperties = getVkQueueFamiliesProperties();
		std::int32_t index = 0;
		for (const auto& properties : familiesProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				return index;
			++index;
		}

		return InvalidIndex;
	}

}