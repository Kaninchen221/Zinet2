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

		CommandBuffer() noexcept = delete;
		CommandBuffer(const CommandBuffer& other) noexcept = delete;
		CommandBuffer(CommandBuffer&& other) noexcept = default;
		~CommandBuffer() noexcept = default;

		CommandBuffer& operator = (const CommandBuffer& other) noexcept = delete;
		CommandBuffer& operator = (CommandBuffer&& other) noexcept = default;

		bool create(const Device& device, const CommandPool& commandPool) noexcept;

		void invalidate() { objectHandle = nullptr; }

		bool reset() noexcept;

		bool begin() noexcept;
		
		bool end() noexcept;
		
		void beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset = { 0, 0 }, const VkClearValue& clearValue = DefaultClearColor) noexcept;
		
		void endRenderPass() noexcept;
		
		void bindPipeline(const Pipeline& pipeline) noexcept;
		
		void setViewport(const VkViewport& viewport) noexcept;
		
		void setScissor(const VkRect2D& scissor) noexcept;
		
		void draw(std::uint32_t vertexCount,
			std::uint32_t instanceCount,
			std::uint32_t firstVertex,
			std::uint32_t firstInstance) noexcept;
	};

	inline bool CommandBuffer::reset() noexcept
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

	inline bool CommandBuffer::begin() noexcept
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

	inline bool CommandBuffer::end() noexcept
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

	inline void CommandBuffer::endRenderPass() noexcept
	{
		vkCmdEndRenderPass(objectHandle);
	}

	inline void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
	{
		vkCmdSetViewport(objectHandle, 0, 1, &viewport);
	}

	inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
	{
		vkCmdSetScissor(objectHandle, 0, 1, &scissor);
	}

	inline void CommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept
	{
		vkCmdDraw(objectHandle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

}