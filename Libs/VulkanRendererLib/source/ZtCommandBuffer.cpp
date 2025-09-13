#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtPipeline.hpp"

namespace zt::vulkan_renderer
{

	bool CommandBuffer::create(const Device& device, const CommandPool& commandPool)
	{
		if (isValid())
			return true;

		const VkCommandBufferAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = commandPool.get(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		const auto result = vkAllocateCommandBuffers(device.get(), &allocInfo, &get());
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

	void CommandBuffer::destroy(const Device& device, const CommandPool& commandPool) noexcept
	{
		if (isValid())
		{
			vkFreeCommandBuffers(device.get(), commandPool.get(), 1u, &get());
			invalidateInternal();
		}
	}

	void CommandBuffer::beginRenderPass(
		const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset, const VkClearValue& clearValue) noexcept
	{
		const VkRenderPassBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = renderPass.get(),
			.framebuffer = framebuffer.get(),
			.renderArea = { { offset.x, offset.y }, extent },
			.clearValueCount = 1,
			.pClearValues = &clearValue
		};

		vkCmdBeginRenderPass(get(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::bindPipeline(const Pipeline& pipeline) noexcept
	{
		vkCmdBindPipeline(get(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
	}
}