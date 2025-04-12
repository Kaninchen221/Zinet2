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

	std::uint32_t PhysicalDevice::getQueueFamilyIndexForPresent() const noexcept
	{
		const auto familiesProperties = getVkQueueFamiliesProperties();
		std::uint32_t index = 0;
		for (const auto& properties : familiesProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				return index;
			++index;
		}

		return InvalidIndex;
	}

	std::uint32_t PhysicalDevice::getQueueFamilyIndexForSurface(const Surface& surface) const noexcept
	{
		const auto familiesProperties = getVkQueueFamiliesProperties();
		std::uint32_t index = 0;
		for ([[maybe_unused]] const auto& properties : familiesProperties)
		{
			VkBool32 surfaceSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(objectHandle, index, surface.get(), &surfaceSupport);
			if (surfaceSupport)
				return index;

			++index;
		}

		return InvalidIndex;

	}

	Device PhysicalDevice::createDevice(const Surface& surface) noexcept
	{
		const auto createQueueCreateInfo = [](std::uint32_t queueFamilyIndex, std::uint32_t count, const std::vector<float>& priorities)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = count;
			queueCreateInfo.pQueuePriorities = priorities.data();
			return queueCreateInfo;
		};

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::vector<std::uint32_t> queueIndicies;

		const std::uint32_t presentQueueFamilyIndex = getQueueFamilyIndexForPresent();
		if (presentQueueFamilyIndex == InvalidIndex)
		{
			Logger->error("Couldn't get queue family index for present");
			return Device{ nullptr };
		}

		const std::uint32_t surfaceQueueFamilyIndex = surface.isValid() ? getQueueFamilyIndexForSurface(surface) : InvalidIndex;
		if (surface.isValid())
		{
			if (surfaceQueueFamilyIndex == InvalidIndex)
			{
				Logger->error("Couldn't get queue family index for surface");
				return Device{ nullptr };
			}
		}

		std::vector<float> priorities;
		if (presentQueueFamilyIndex == surfaceQueueFamilyIndex)
		{
			priorities.push_back(1.f);
			priorities.push_back(1.f);
			queueCreateInfos.push_back(createQueueCreateInfo(presentQueueFamilyIndex, 2, priorities));
		}
		else
		{
			priorities.push_back(1.f);
			queueCreateInfos.push_back(createQueueCreateInfo(presentQueueFamilyIndex, 1, priorities));
			if (surface.isValid())
			{
				priorities.push_back(1.f);
				queueCreateInfos.push_back(createQueueCreateInfo(surfaceQueueFamilyIndex, 1, priorities));
			}
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
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
			return Device{ device, presentQueueFamilyIndex, surfaceQueueFamilyIndex };
		}
		else
		{
			Logger->error("Couldn't create device, result: {}", static_cast<int>(createResult));
			return Device(nullptr);
		}
	}

}