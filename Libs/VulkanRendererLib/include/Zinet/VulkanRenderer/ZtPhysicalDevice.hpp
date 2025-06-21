#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <vulkan/vulkan.h>

#include <optional>

namespace zt::vulkan_renderer
{
	class Surface;

	// TODO: Cache various data? Like VkPhysicalDeviceProperties or VkPhysicalDeviceFeatures
	class ZINET_VULKAN_RENDERER_API PhysicalDevice : public VulkanObject<VkPhysicalDevice>
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

		void invalidate() { objectHandle = nullptr; }

		VkPhysicalDeviceProperties getVkPhysicalDeviceProperties() const noexcept;

		VkPhysicalDeviceFeatures getVkPhysicalDeviceFeatures() const noexcept;

		static PhysicalDevice TakeBestPhysicalDevice(auto& physicalDevices) noexcept;

		std::vector<VkQueueFamilyProperties> getVkQueuesFamiliesProperties() const noexcept;

		void printVkQueuesFamiliesProperties(const auto& familiesProperties, const Surface& surface) const noexcept;

		std::uint32_t takeQueueFamilyIndexForPresent(std::vector<VkQueueFamilyProperties>& familiesProperties) const noexcept;

		std::uint32_t takeQueueFamilyIndexForSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept;

		std::uint32_t takeQueueFamilyIndexForPresentAndSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const noexcept;

		bool isQueueFamilySupportingSurface(std::uint32_t index, const Surface& surface) const noexcept;

		std::vector<VkExtensionProperties> getDeviceExtensionProperties() const noexcept;

		static std::vector<const char*> GetRequiredExtensions() noexcept;

		VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(const Surface& surface) const noexcept;

		std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(const Surface& surface) const noexcept;

		std::vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(const Surface& surface) const noexcept;
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

			Logger->info("Test physical device: {}", properties.deviceName);
			Logger->info("API version: {}", properties.apiVersion);
			Logger->info("Driver version: {}", properties.driverVersion);

			const auto requiredExtensions = PhysicalDevice::GetRequiredExtensions();

			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				bool foundAllRequiredExtensions = true;
				const auto deviceExtensions = physicalDevice.getDeviceExtensionProperties();
				for (auto requiredExtension : requiredExtensions)
				{
					const auto predicate = [requiredExtension](const VkExtensionProperties& extension) { return std::string_view(extension.extensionName) == requiredExtension; };
					if (std::find_if(std::begin(deviceExtensions), std::end(deviceExtensions), predicate) == std::end(deviceExtensions))
					{
						Logger->info("Device don't support extension: {}", requiredExtension);
						foundAllRequiredExtensions = false;
						break;
					}
					else
					{
						Logger->info("Device support extension: {}", requiredExtension);
					}
				}

				if (foundAllRequiredExtensions)
				{
					Logger->info("Found physical device: {} with device type: {}", properties.deviceName, static_cast<std::int32_t>(properties.deviceType));
					return std::move(physicalDevice);
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