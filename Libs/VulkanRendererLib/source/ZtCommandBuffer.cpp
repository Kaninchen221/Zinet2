#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtPipeline.hpp"

namespace zt::vulkan_renderer
{

	bool CommandBuffer::create(const Device& device, const CommandPool& commandPool) noexcept
	{
		if (isValid())
			return true;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		const auto result = vkAllocateCommandBuffers(device.get(), &allocInfo, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create command buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool CommandBuffer::reset() noexcept
	{
		const auto result = vkResetCommandBuffer(objectHandle, 0);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't reset, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool CommandBuffer::begin() noexcept
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		const auto result = vkBeginCommandBuffer(objectHandle, &beginInfo);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't begin, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool CommandBuffer::end() noexcept
	{
		const auto result = vkEndCommandBuffer(objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't end, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void CommandBuffer::beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset, const VkClearValue& clearValue) noexcept
	{
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass.get();
		beginInfo.framebuffer = framebuffer.get();
		beginInfo.renderArea.offset = { offset.x, offset.y };
		beginInfo.renderArea.extent = extent;
		VkClearValue clearColor = clearValue;
		beginInfo.clearValueCount = 1;
		beginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(objectHandle, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::endRenderPass() noexcept
	{
		vkCmdEndRenderPass(objectHandle);
	}

	void CommandBuffer::bindPipeline(const Pipeline& pipeline) noexcept
	{
		vkCmdBindPipeline(objectHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
	}

	void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
	{
		vkCmdSetViewport(objectHandle, 0, 1, &viewport);
	}

	void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
	{
		vkCmdSetScissor(objectHandle, 0, 1, &scissor);
	}

	void CommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept
	{
		vkCmdDraw(objectHandle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

}