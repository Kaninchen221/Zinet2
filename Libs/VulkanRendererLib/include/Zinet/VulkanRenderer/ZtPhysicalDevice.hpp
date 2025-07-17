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

		PhysicalDevice() ZINET_API_POST = delete;
		PhysicalDevice(const PhysicalDevice& other) ZINET_API_POST = delete;
		PhysicalDevice(PhysicalDevice&& other) ZINET_API_POST = default;
		~PhysicalDevice() ZINET_API_POST = default;

		PhysicalDevice& operator = (const PhysicalDevice& other) ZINET_API_POST = delete;
		PhysicalDevice& operator = (PhysicalDevice&& other) ZINET_API_POST = default;

		void invalidate() { objectHandle = nullptr; }

		VkPhysicalDeviceProperties getVkPhysicalDeviceProperties() const ZINET_API_POST;

		VkPhysicalDeviceFeatures getVkPhysicalDeviceFeatures() const ZINET_API_POST;

		static PhysicalDevice TakeBestPhysicalDevice(auto& physicalDevices) ZINET_API_POST;

		std::vector<VkQueueFamilyProperties> getVkQueuesFamiliesProperties() const ZINET_API_POST;

		void printVkQueuesFamiliesProperties(const auto& familiesProperties, const Surface& surface) const ZINET_API_POST;

		std::uint32_t takeQueueFamilyIndexForPresent(std::vector<VkQueueFamilyProperties>& familiesProperties) const ZINET_API_POST;

		std::uint32_t takeQueueFamilyIndexForSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const ZINET_API_POST;

		std::uint32_t takeQueueFamilyIndexForPresentAndSurface(std::vector<VkQueueFamilyProperties>& familiesProperties, const Surface& surface) const ZINET_API_POST;

		bool isQueueFamilySupportingSurface(std::uint32_t index, const Surface& surface) const ZINET_API_POST;

		std::vector<VkExtensionProperties> getDeviceExtensionProperties() const ZINET_API_POST;

		static std::vector<const char*> GetRequiredExtensions() ZINET_API_POST;

		VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(const Surface& surface) const ZINET_API_POST;

		std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(const Surface& surface) const ZINET_API_POST;

		std::vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(const Surface& surface) const ZINET_API_POST;
	};

	inline void PhysicalDevice::printVkQueuesFamiliesProperties(const auto& familiesProperties, const Surface& surface) const ZINET_API_POST
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

	inline PhysicalDevice PhysicalDevice::TakeBestPhysicalDevice(auto& physicalDevices) ZINET_API_POST
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