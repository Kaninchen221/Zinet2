#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <optional>

namespace zt::vulkan_renderer
{
	class Surface;

	class ZINET_VULKAN_RENDERER_API PhysicalDevice : public VulkanObject<VkPhysicalDevice, false>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPhysicalDevice");

	public:

		PhysicalDevice(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		PhysicalDevice() noexcept = delete;
		PhysicalDevice(const PhysicalDevice& other) noexcept = delete;
		PhysicalDevice(PhysicalDevice&& other) noexcept = default;
		~PhysicalDevice() noexcept = default;

		PhysicalDevice& operator = (const PhysicalDevice& other) noexcept = delete;
		PhysicalDevice& operator = (PhysicalDevice&& other) noexcept = default;

		VkPhysicalDeviceProperties getVkPhysicalDeviceProperties() const noexcept;

		VkPhysicalDeviceFeatures getVkPhysicalDeviceFeatures() const noexcept;

		static PhysicalDevice TakeBestPhysicalDevice(auto& physicalDevices) noexcept;

		std::vector<VkQueueFamilyProperties> getVkQueuesFamiliesProperties() const noexcept;

		void printVkQueuesFamiliesProperties(const auto& familiesProperties, const Surface& surface) const noexcept;

		std::uint32_t takeQueueFamilyIndexForPresent(std::vector<VkQueueFamilyProperties>& familiesProperties) const noexcept;

		std::uint32_t takeQueueFamilyIndexForSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept;

		std::uint32_t takeQueueFamilyIndexForPresentAndSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept;

		bool isQueueFamilySupportingSurface(std::uint32_t index, const Surface& surface) const noexcept;

		const std::vector<VkExtensionProperties> getDeviceExtensionProperties() const noexcept;

		const std::vector<const char*> getRequiredExtensions() const noexcept;

		const VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(const Surface& surface) const noexcept;

		const std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(const Surface& surface) const noexcept;

		const std::vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(const Surface& surface) const noexcept;
	};

	inline void PhysicalDevice::printVkQueuesFamiliesProperties(const auto& familiesProperties, const Surface& surface) const noexcept
	{
		Logger->info("Print VkQueuesFamiliesProperties");

		std::uint32_t index = 0;
		for (const auto& properties : familiesProperties)
		{
			if (surface.isValid())
			{
				const bool supportsSurface = isQueueFamilySupportingSurface(index, surface);
				Logger->info("\tIndex: {} QueueCount: {} SurfaceSupport: {}", index, properties.queueCount, supportsSurface);
			}
			else
			{
				Logger->info("\tIndex: {} QueueCount: {}", index, properties.queueCount);
			}
			++index;
		}
	}

	inline PhysicalDevice PhysicalDevice::TakeBestPhysicalDevice(auto& physicalDevices) noexcept
	{
		Logger->info("Get best physical device");

		for (auto& physicalDevice : physicalDevices)
		{
			if (!physicalDevice.isValid())
				continue;
		
			const auto properties = physicalDevice.getVkPhysicalDeviceProperties();
			const auto features = physicalDevice.getVkPhysicalDeviceFeatures();

			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				const auto deviceExtensionProperties = physicalDevice.getDeviceExtensionProperties();
				for (const auto& deviceExtension : deviceExtensionProperties)
				{
					if (std::string_view(deviceExtension.extensionName) == VK_KHR_SWAPCHAIN_EXTENSION_NAME)
					{
						Logger->info("Found physical device: {} with device type: {}", properties.deviceName, static_cast<std::int32_t>(properties.deviceType));
						return std::move(physicalDevice);
					}
				}
			}
		}

		for (auto& physicalDevice : physicalDevices)
		{
			if (!physicalDevice.isValid())
				continue;

			const auto properties = physicalDevice.getVkPhysicalDeviceProperties();
			Logger->info("Take first physical device: {} with device type: {}", properties.deviceName, static_cast<std::int32_t>(properties.deviceType));

			return std::move(physicalDevice);
		}

		return PhysicalDevice(nullptr);
	}
}