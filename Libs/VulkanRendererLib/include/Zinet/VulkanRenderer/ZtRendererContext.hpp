#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Device;
	class VMA;

	class ZINET_VULKAN_RENDERER_API RendererContext
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRRendererContext");

	public:

		RendererContext() ZINET_API_POST = default;
		RendererContext(const RendererContext& other) ZINET_API_POST = delete;
		RendererContext(RendererContext&& other) ZINET_API_POST = default;
		~RendererContext() ZINET_API_POST = default;

		RendererContext& operator = (const RendererContext& other) ZINET_API_POST = delete;
		RendererContext& operator = (RendererContext&& other) ZINET_API_POST = default;

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Surface surface{ nullptr };
		Device device{ nullptr };
		VMA vma{ nullptr };
		SwapChain swapChain{ nullptr };
		Queue queue{ nullptr };
		CommandPool commandPool{ nullptr };

		Semaphore imageAvailableSemaphore{ nullptr };
		Semaphore renderFinishedSemaphore{ nullptr };
		Fence fence{ nullptr };

		std::vector<VkImage> images;
		std::vector<ImageView> imageViews;
		std::vector<Framebuffer> framebuffers;

		uint32_t currentFramebufferIndex{};

		RenderPass renderPass{ nullptr };

		bool create(wd::Window& window) ZINET_API_POST;

		void destroy() ZINET_API_POST;

		void windowResized(const Vector2i& size) ZINET_API_POST;

	};
}