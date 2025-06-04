#pragma once

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"

namespace zt::vulkan_renderer
{
	const auto SubmitSingleCommandBufferWaitIdle = [](const Device& device, const Queue& queue, const CommandPool& commandPool, std::invocable<const CommandBuffer&> auto commands) -> bool
	{
		CommandBuffer commandBuffer{ nullptr };
		if (!commandBuffer.create(device, commandPool))
			return false;

		if (!commandBuffer.begin())
			return false;

		std::invoke(commands, commandBuffer);

		if (!commandBuffer.end())
			return false;

		const std::array vkCommandBuffers = { commandBuffer.get() };

		auto submitInfo = CommandBuffer::GetDefaultSubmitInfo();
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = vkCommandBuffers.data();

		if (!queue.submit(std::array{ submitInfo }))
			return false;

		if (!queue.waitIdle())
			return false;

		commandBuffer.destroy(device, commandPool);

		return true;
	};
}