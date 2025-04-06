#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API PhysicalDevice : public VulkanObject<VkPhysicalDevice, false>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPhysicalDevice");

	public:

		inline static constexpr std::int32_t InvalidIndex = -1;

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

		static PhysicalDevice GetBestDevice(auto& physicalDevices) noexcept;

		std::vector<VkQueueFamilyProperties> getVkQueueFamiliesProperties() const noexcept;

		std::int32_t getQueueFamilyIndexForPresent() const noexcept;
	};

	inline PhysicalDevice PhysicalDevice::GetBestDevice(auto& physicalDevices) noexcept
	{
		Logger->info("Get best physical device");

		for (auto& physicalDevice : physicalDevices)
		{
			if (!physicalDevice.isValid())
				continue;
		
			auto properties = physicalDevice.getVkPhysicalDeviceProperties();
			auto features = physicalDevice.getVkPhysicalDeviceFeatures();
		
			Logger->info("Physical device: {} with device type: {}", properties.deviceName, static_cast<std::int32_t>(properties.deviceType));
		
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				return std::move(physicalDevice);
		}

		for (auto& physicalDevice : physicalDevices)
		{
			if (physicalDevice.isValid())
				return std::move(physicalDevice);
		}

		return PhysicalDevice(nullptr);
	}
}