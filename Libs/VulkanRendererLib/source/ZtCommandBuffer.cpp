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

	void CommandBuffer::bindPipeline(const Pipeline& pipeline) noexcept
	{
		vkCmdBindPipeline(objectHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
	}
}