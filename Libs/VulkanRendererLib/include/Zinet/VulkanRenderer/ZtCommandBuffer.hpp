#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::vulkan_renderer
{
	class Device;
	class CommandPool;
	class RenderPass;
	class Framebuffer;
	class Pipeline;

	class ZINET_VULKAN_RENDERER_API CommandBuffer : public VulkanObject<VkCommandBuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRCommandBuffer");

	public:

		inline static const auto DefaultClearColor = VkClearValue{ VkClearColorValue{} };

		CommandBuffer(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		CommandBuffer() ZINET_API_POST = delete;
		CommandBuffer(const CommandBuffer& other) ZINET_API_POST = delete;
		CommandBuffer(CommandBuffer&& other) ZINET_API_POST = default;
		~CommandBuffer() ZINET_API_POST = default;

		CommandBuffer& operator = (const CommandBuffer& other) ZINET_API_POST = delete;
		CommandBuffer& operator = (CommandBuffer&& other) ZINET_API_POST = default;

		bool create(const Device& device, const CommandPool& commandPool) ZINET_API_POST;

		void destroy(const Device& device, const CommandPool& commandPool) ZINET_API_POST;

		void invalidate() ZINET_API_POST { objectHandle = nullptr; }

		bool reset() ZINET_API_POST;

		bool begin() ZINET_API_POST;
		
		bool end() ZINET_API_POST;
		
		void beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset = { 0, 0 }, const VkClearValue& clearValue = DefaultClearColor) ZINET_API_POST;
		
		void endRenderPass() ZINET_API_POST;
		
		void bindPipeline(const Pipeline& pipeline) ZINET_API_POST;
		
		void setViewport(const VkViewport& viewport) ZINET_API_POST;
		
		void setScissor(const VkRect2D& scissor) ZINET_API_POST;
		
		void draw(std::uint32_t vertexCount,
			std::uint32_t instanceCount,
			std::uint32_t firstVertex,
			std::uint32_t firstInstance) ZINET_API_POST;

		static VkSubmitInfo GetDefaultSubmitInfo() ZINET_API_POST;
	};

	inline bool CommandBuffer::reset() ZINET_API_POST
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

	inline bool CommandBuffer::begin() ZINET_API_POST
	{
		const VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr,
		};

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

	inline bool CommandBuffer::end() ZINET_API_POST
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

	inline void CommandBuffer::endRenderPass() ZINET_API_POST
	{
		vkCmdEndRenderPass(objectHandle);
	}

	inline void CommandBuffer::setViewport(const VkViewport& viewport) ZINET_API_POST
	{
		vkCmdSetViewport(objectHandle, 0, 1, &viewport);
	}

	inline void CommandBuffer::setScissor(const VkRect2D& scissor) ZINET_API_POST
	{
		vkCmdSetScissor(objectHandle, 0, 1, &scissor);
	}

	inline void CommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) ZINET_API_POST
	{
		vkCmdDraw(objectHandle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	inline VkSubmitInfo CommandBuffer::GetDefaultSubmitInfo() ZINET_API_POST
	{
		return
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 0,
			.pWaitSemaphores = nullptr,
			.pWaitDstStageMask = nullptr,
			.commandBufferCount = 0,
			.pCommandBuffers = nullptr,
			.signalSemaphoreCount = 0,
			.pSignalSemaphores = nullptr
		};
	}
}