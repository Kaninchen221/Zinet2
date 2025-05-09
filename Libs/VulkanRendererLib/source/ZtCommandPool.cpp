#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"

namespace zt::vulkan_renderer
{

	bool CommandPool::create(const Device& device, const Queue& queue) noexcept
	{
		if (isValid())
			return false;

		const VkCommandPoolCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = queue.getQueueFamilyIndex()
		};

		const auto result = vkCreateCommandPool(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create command pool, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void CommandPool::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyCommandPool(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}