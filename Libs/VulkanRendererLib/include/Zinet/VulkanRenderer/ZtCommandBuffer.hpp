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

	class ZINET_VULKAN_RENDERER_API CommandBuffer : public VulkanObject<VkCommandBuffer, false>
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

		bool begin() noexcept;

		bool end() noexcept;

		void beginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const VkExtent2D& extent, const Vector2i& offset = { 0, 0 }, const VkClearValue& clearValue = DefaultClearColor) noexcept;

		void endRenderPass() noexcept;
	};
}