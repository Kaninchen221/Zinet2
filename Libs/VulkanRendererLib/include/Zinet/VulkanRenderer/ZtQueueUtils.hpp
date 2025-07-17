#pragma once

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtConcepts.hpp"

namespace zt::vulkan_renderer
{
	auto SubmitSingleCommandBufferWaitIdle(const RendererContext& rendererContext, IsCommandsT auto commands) -> bool
	{
		return SubmitSingleCommandBufferWaitIdle(rendererContext.device, rendererContext.queue, rendererContext.commandPool, commands);
	};

	auto SubmitSingleCommandBufferWaitIdle(const Device& device, const Queue& queue, const CommandPool& commandPool, IsCommandsT auto commands) -> bool
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

		if (!device.waitIdle())
			return false;

		commandBuffer.destroy(device, commandPool);

		return true;
	};
}