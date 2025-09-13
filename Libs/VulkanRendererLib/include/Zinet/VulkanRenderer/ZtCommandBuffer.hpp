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

	class  CommandBuffer : public VulkanObject<VkCommandBuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRCommandBuffer");

	public:

		inline static const VkClearValue DefaultClearColor = VkClearValue{ VkClearColorValue{} };

		CommandBuffer(VulcanType newHandle) noexcept
			: VulkanObject(newHandle) {}

		CommandBuffer() noexcept = delete;
		CommandBuffer(const CommandBuffer& other) noexcept = delete;
		CommandBuffer(CommandBuffer&& other) noexcept = default;
		~CommandBuffer() noexcept = default;

		CommandBuffer& operator = (const CommandBuffer& other) noexcept = delete;
		CommandBuffer& operator = (CommandBuffer&& other) noexcept = default;

		bool create(const Device& device, const CommandPool& commandPool);

		void destroy(const Device& device, const CommandPool& commandPool) noexcept;

		void invalidate() noexcept { invalidateInternal(); }

		bool reset();

		bool begin();
		
		bool end();
		
		void beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset = { 0, 0 }, const VkClearValue& clearValue = DefaultClearColor) noexcept;
		
		void endRenderPass() noexcept;
		
		void bindPipeline(const Pipeline& pipeline) noexcept;
		
		void setViewport(const VkViewport& viewport) noexcept;
		
		void setScissor(const VkRect2D& scissor) noexcept;
		
		void draw(std::uint32_t vertexCount,
			std::uint32_t instanceCount,
			std::uint32_t firstVertex,
			std::uint32_t firstInstance) noexcept;

		static VkSubmitInfo GetDefaultSubmitInfo() noexcept;
	};

	inline bool CommandBuffer::reset()
	{
		const auto result = vkResetCommandBuffer(get(), VkCommandBufferResetFlags{});
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

	inline bool CommandBuffer::begin()
	{
		const VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr,
		};

		const auto result = vkBeginCommandBuffer(get(), &beginInfo);
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

	inline bool CommandBuffer::end()
	{
		const auto result = vkEndCommandBuffer(get());
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
		vkCmdEndRenderPass(get());
	}

	inline void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
	{
		vkCmdSetViewport(get(), 0, 1, &viewport);
	}

	inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
	{
		vkCmdSetScissor(get(), 0, 1, &scissor);
	}

	inline void CommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept
	{
		vkCmdDraw(get(), vertexCount, instanceCount, firstVertex, firstInstance);
	}

	inline VkSubmitInfo CommandBuffer::GetDefaultSubmitInfo() noexcept
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