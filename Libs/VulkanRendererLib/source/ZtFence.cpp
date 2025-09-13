#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	bool Fence::create(const Device& device, bool signaled)
	{
		if (isValid())
			return false;

		const VkFenceCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : VkFenceCreateFlags{}
		};

		const auto result = vkCreateFence(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create fence, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Fence::destroy(const Device& device)
	{
		if (isValid())
		{
			vkDestroyFence(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	bool Fence::wait(const Device& device, std::uint64_t timeout) const
	{
		const auto result = vkWaitForFences(device.get(), 1u, &objectHandle, VK_TRUE, timeout);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't wait for fence, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool Fence::isSignaled(const Device& device) const
	{
		const auto result = vkGetFenceStatus(device.get(), objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else if (result == VK_NOT_READY)
		{
			return false;
		}
		else
		{
			Logger->error("Couldn't get fence status, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool Fence::reset(const Device& device)
	{
		const auto result = vkResetFences(device.get(), 1, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't get fence status, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

}