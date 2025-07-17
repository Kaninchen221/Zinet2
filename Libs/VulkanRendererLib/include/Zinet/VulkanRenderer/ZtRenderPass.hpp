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

		RenderPassCreateInfo& operator = (const RenderPassCreateInfo& other) ZINET_API_POST;
	};

	class ZINET_VULKAN_RENDERER_API RenderPass : public VulkanObject<VkRenderPass>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRRenderPass");

	public:

		RenderPass(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		RenderPass() ZINET_API_POST = delete;
		RenderPass(const RenderPass& other) ZINET_API_POST = delete;
		RenderPass(RenderPass&& other) ZINET_API_POST = default;
		~RenderPass() ZINET_API_POST = default;

		RenderPass& operator = (const RenderPass& other) ZINET_API_POST = delete;
		RenderPass& operator = (RenderPass&& other) ZINET_API_POST = default;

		static RenderPassCreateInfo GetPresentCreateInfo(VkFormat format) ZINET_API_POST;

		static RenderPassCreateInfo GetDrawCreateInfo(VkFormat format) ZINET_API_POST;

		bool create(const Device& device, const RenderPassCreateInfo& createInfo) ZINET_API_POST;

		bool recreate(const Device& device) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	protected:

		RenderPassCreateInfo cachedCreateInfo;

	};
}