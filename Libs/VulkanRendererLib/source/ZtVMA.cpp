#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{

	bool VMA::create(const Device& device, const PhysicalDevice& physicalDevice, const Instance& instance) noexcept
	{
		if (isValid())
			return false;

		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
		vulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2;

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorCreateInfo.physicalDevice = physicalDevice.get();
		allocatorCreateInfo.device = device.get();
		allocatorCreateInfo.instance = instance.get();
		allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

		if (core::Contains(physicalDevice.GetRequiredExtensions(), std::string_view(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME)))
			allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		else
			allocatorCreateInfo.flags = 0;

		const auto result = vmaCreateAllocator(&allocatorCreateInfo, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Can't create VmaAllocator, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void VMA::destroy() noexcept
	{
		if (isValid())
		{
			vmaDestroyAllocator(objectHandle);
			objectHandle = nullptr;
		}
	}

}