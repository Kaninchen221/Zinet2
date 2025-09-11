#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;

	class DisplayImage
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DisplayImage");

	public:

		VkImage image{};
		ImageView imageView{ nullptr };
		Framebuffer framebuffer{ nullptr };
		Fence fence{ nullptr };
		CommandBuffer commandBuffer{ nullptr };

		bool create(RendererContext& rendererContext, VkImage swapChainImage);

		void destroy(RendererContext& rendererContext);

	};
}