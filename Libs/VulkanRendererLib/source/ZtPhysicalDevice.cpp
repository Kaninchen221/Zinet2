#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

#include "Zinet/Window/ZtGLFW.hpp"

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

	std::vector<VkExtensionProperties> PhysicalDevice::getDeviceExtensionProperties() const noexcept
	{
		std::uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(objectHandle, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(objectHandle, nullptr, &extensionCount, extensions.data());

		return extensions;
	}

	std::vector<const char*> PhysicalDevice::GetRequiredExtensions() noexcept
	{
		if (!wd::GLFW::IsInitialized())
			return {};

		std::vector<const char*> result{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME };

#if VMA_MEMORY_BUDGET == 1
		result.push_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
#endif

		return result;
	}
	
	VkSurfaceCapabilitiesKHR PhysicalDevice::getPhysicalDeviceSurfaceCapabilities(const Surface& surface) const noexcept
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(objectHandle, surface.get(), &surfaceCapabilities);
		return surfaceCapabilities;
	}

	std::vector<VkSurfaceFormatKHR> PhysicalDevice::getPhysicalDeviceSurfaceFormats(const Surface& surface) const noexcept
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

	std::vector<VkPresentModeKHR> PhysicalDevice::getPhysicalDeviceSurfacePresentModes(const Surface& surface) const noexcept
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