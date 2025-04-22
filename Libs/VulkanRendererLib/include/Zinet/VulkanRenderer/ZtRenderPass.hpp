#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API RenderPass : public VulkanObject<VkRenderPass>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRRenderPass");

	public:

		RenderPass(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		RenderPass() noexcept = delete;
		RenderPass(const RenderPass& other) noexcept = delete;
		RenderPass(RenderPass&& other) noexcept = default;
		~RenderPass() noexcept = default;

		RenderPass& operator = (const RenderPass& other) noexcept = delete;
		RenderPass& operator = (RenderPass&& other) noexcept = default;
	
		bool createForPresent(const Device& device, const VkFormat format) noexcept;

		bool createForDraw(const Device& device, const VkFormat format) noexcept;

		void destroy(const Device& device) noexcept;

	protected:

		bool createInternal(const Device& device, const VkRenderPassCreateInfo& createInfo) noexcept;

	};
}