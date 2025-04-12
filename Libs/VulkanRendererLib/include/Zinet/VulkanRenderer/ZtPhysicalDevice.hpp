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

		std::vector<VkQueueFamilyProperties> getVkQueueFamiliesProperties() const noexcept;

		std::uint32_t getQueueFamilyIndexForPresent() const noexcept;

		std::uint32_t getQueueFamilyIndexForSurface(const Surface& surface) const noexcept;

		Device createDevice(const Surface& surface = Surface{ nullptr }) noexcept;
	};

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
				Logger->info("Found physical device: {} with device type: {}", properties.deviceName, static_cast<std::int32_t>(properties.deviceType));
				return std::move(physicalDevice);
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