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
	class RendererContext;

	class DisplayImage
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DisplayImage");

	public:

		VkImage image{};
		ImageView imageView{ nullptr };
		Framebuffer framebuffer{ nullptr };

		bool create(RendererContext& rendererContext, VkImage swapChainImage);

		void destroy(RendererContext& rendererContext);
	};

	class  RendererContext
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::VRRendererContext");

	public:

		RendererContext() noexcept = default;
		RendererContext(const RendererContext& other) noexcept = delete;
		RendererContext(RendererContext&& other) noexcept = default;
		~RendererContext() noexcept = default;

		RendererContext& operator = (const RendererContext& other) noexcept = delete;
		RendererContext& operator = (RendererContext&& other) noexcept = default;

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

		using DisplayImages = std::vector<DisplayImage>;
		DisplayImages displayImages;

		bool createDisplayImages();

		// TODO: Refactor
		auto& getPreviousDisplayImage()
		{
			if (currentFramebufferIndex == 0)
				return displayImages.back();

			return displayImages[currentFramebufferIndex - 1];
		}

		auto& getPreviousDisplayImage() const
		{
			if (currentFramebufferIndex == 0)
				return displayImages.back();

			return displayImages[currentFramebufferIndex - 1];
		}

		auto& getCurrentDisplayImage() { return displayImages[currentFramebufferIndex]; }
		auto& getCurrentDisplayImage() const { return displayImages[currentFramebufferIndex]; }

		uint32_t currentFramebufferIndex{};

		RenderPass renderPass{ nullptr };

		bool create(wd::Window& window);

		void destroy();

		void windowResized(const Vector2i& size);

	};
}