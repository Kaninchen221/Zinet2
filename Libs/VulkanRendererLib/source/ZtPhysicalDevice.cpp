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

	Device PhysicalDevice::createDeviceForPresent() noexcept
	{
		const std::int32_t queueFamilyIndex = getQueueFamilyIndexForPresent();
		if (queueFamilyIndex == InvalidIndex)
		{
			Logger->error("Couldn't get queue family index for present");
			return Device{ nullptr };
		}

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;

		const float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;

		// Deprecated and ignored
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;

		VkDevice device = nullptr;
		const auto createResult = vkCreateDevice(objectHandle, &createInfo, nullptr, &device);

		if (createResult == VK_SUCCESS)
		{
			return Device{ device, queueFamilyIndex };
		}
		else
		{
			Logger->error("Couldn't create device, result: {}", static_cast<int>(createResult));
			return Device(nullptr);
		}
	}

}