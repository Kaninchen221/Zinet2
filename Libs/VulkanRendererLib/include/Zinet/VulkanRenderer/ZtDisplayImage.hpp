#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"

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
		CommandBuffer commandBuffer{ nullptr };

		Fence fence{ nullptr };

		Semaphore imageAvailableSemaphore{ nullptr };
		Semaphore renderFinishedSemaphore{ nullptr };

		bool create(RendererContext& rendererContext, VkImage swapChainImage, uint32_t index);

		void destroy(RendererContext& rendererContext);

	};
}