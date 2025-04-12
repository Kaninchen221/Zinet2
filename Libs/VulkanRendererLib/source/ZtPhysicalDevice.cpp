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

	std::vector<VkQueueFamilyProperties> PhysicalDevice::getVkQueuesFamiliesProperties() const noexcept
	{
		std::uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(objectHandle, &count, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(count);
		vkGetPhysicalDeviceQueueFamilyProperties(objectHandle, &count, queueFamilies.data());

		return queueFamilies;
	}

	std::uint32_t PhysicalDevice::takeQueueFamilyIndexForPresent(std::vector<VkQueueFamilyProperties>& familiesProperties) const noexcept
	{
		std::uint32_t index = 0;
		for (auto& properties : familiesProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && properties.queueCount > 0)
			{
				properties.queueCount -= 1;
				Logger->info("Found queue family index for present. Family index: {}, Queues left for that family: {}", index, properties.queueCount);
				return index;
			}
			++index;
		}

		Logger->error("Couldn't find queue family index for present");
		return InvalidIndex;
	}

	std::uint32_t PhysicalDevice::takeQueueFamilyIndexForSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept
	{
		std::uint32_t index = 0;
		for (auto& properties : familiesProperties)
		{
			const bool supportsSurface = isQueueFamilySupportingSurface(index, surface);
			if (supportsSurface && properties.queueCount > 0)
			{
				properties.queueCount -= 1;
				Logger->info("Found queue family index for surface. Family index: {}, Queues left for that family: {}", index, properties.queueCount);
				return index;
			}

			++index;
		}

		Logger->info("Couldn't find queue family index for surface");
		return InvalidIndex;
	}

	std::uint32_t PhysicalDevice::takeQueueFamilyIndexForPresentAndSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept
	{
		std::uint32_t index = 0;
		for (auto& properties : familiesProperties)
		{
			const bool supportsSurface = isQueueFamilySupportingSurface(index, surface);
			if (supportsSurface && properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && properties.queueCount > 0)
			{
				properties.queueCount -= 1;
				Logger->info("Found queue family index for present and surface. Family index: {}, Queues left for that family: {}", index, properties.queueCount);
				return index;
			}
			++index;
		}

		Logger->error("Couldn't find queue family index for present and surface");
		return InvalidIndex;
	}

	bool PhysicalDevice::isQueueFamilySupportingSurface(std::uint32_t index, const Surface& surface) const noexcept
	{
		VkBool32 surfaceSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(objectHandle, index, surface.get(), &surfaceSupport);
		return surfaceSupport;
	}

	const std::vector<VkExtensionProperties> PhysicalDevice::getDeviceExtensionProperties() const noexcept
	{
		std::uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(objectHandle, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(objectHandle, nullptr, &extensionCount, extensions.data());

		return extensions;
	}

	const std::vector<const char*> PhysicalDevice::getRequiredExtensions() const noexcept
	{
		return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
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

		auto queuesFamiliesProperties = getVkQueuesFamiliesProperties();
		printVkQueuesFamiliesProperties(queuesFamiliesProperties, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::vector<std::uint32_t> queueIndicies;

		const std::uint32_t queueFamilyIndex = 
			surface.isValid() ? takeQueueFamilyIndexForPresentAndSurface(queuesFamiliesProperties, surface) : takeQueueFamilyIndexForPresent(queuesFamiliesProperties);

		if (queueFamilyIndex == InvalidIndex)
		{
			Logger->error("Couldn't get queue family index for present");
			return Device{ nullptr };
		}

		std::vector<float> priorities;
		priorities.push_back(1.f);
		queueCreateInfos.push_back(createQueueCreateInfo(queueFamilyIndex, 1, priorities));

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		const auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

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
			Logger->error("Couldn't create device, result: {}", static_cast<std::int32_t>(createResult));
			return Device(nullptr);
		}
	}

	const VkSurfaceCapabilitiesKHR PhysicalDevice::getPhysicalDeviceSurfaceCapabilities(const Surface& surface) const noexcept
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(objectHandle, surface.get(), &surfaceCapabilities);
		return surfaceCapabilities;
	}

	const std::vector<VkSurfaceFormatKHR> PhysicalDevice::getPhysicalDeviceSurfaceFormats(const Surface& surface) const noexcept
	{
		std::uint32_t count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(objectHandle, surface.get(), &count, nullptr);

		if (count == 0)
			return {};

		std::vector<VkSurfaceFormatKHR> formats;
		formats.resize(count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(objectHandle, surface.get(), &count, formats.data());

		return formats;
	}

	const std::vector<VkPresentModeKHR> PhysicalDevice::getPhysicalDeviceSurfacePresentModes(const Surface& surface) const noexcept
	{
		std::uint32_t count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(objectHandle, surface.get(), &count, nullptr);

		if (count == 0)
			return {};

		std::vector<VkPresentModeKHR> presentModes;
		presentModes.resize(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(objectHandle, surface.get(), &count, presentModes.data());

		return presentModes;
	}

}