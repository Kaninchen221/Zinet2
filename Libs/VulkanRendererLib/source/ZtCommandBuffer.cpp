#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtPipeline.hpp"

namespace zt::vulkan_renderer
{

	bool CommandBuffer::create(const Device& device, const CommandPool& commandPool) ZINET_API_POST
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

	void CommandBuffer::destroy(const Device& device, const CommandPool& commandPool) ZINET_API_POST
	{
		if (isValid())
		{
			vkFreeCommandBuffers(device.get(), commandPool.get(), 1u, &objectHandle);
			objectHandle = nullptr;
		}
	}

	void CommandBuffer::beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset, const VkClearValue& clearValue) ZINET_API_POST
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

		vkCmdBeginRenderPass(objectHandle, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::bindPipeline(const Pipeline& pipeline) ZINET_API_POST
	{
		vkCmdBindPipeline(objectHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
	}
}