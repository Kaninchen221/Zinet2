#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	struct ZINET_VULKAN_RENDERER_API RenderPassCreateInfo
	{
		VkAttachmentDescription colorAttachmentDescription{};
		VkAttachmentReference colorAttachmentReference{};
		VkSubpassDescription subpassDescription{};
		VkRenderPassCreateInfo vkCreateInfo{};
	};

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

		static RenderPassCreateInfo GetPresentCreateInfo(VkFormat format) noexcept;

		static RenderPassCreateInfo GetDrawCreateInfo(VkFormat format) noexcept;

		bool create(const Device& device, const RenderPassCreateInfo& createInfo) noexcept;

		void destroy(const Device& device) noexcept;

	protected:

	};
}