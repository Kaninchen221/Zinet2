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
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtDisplayImage.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class  RendererContext
	{
		friend class VulkanRenderer;

	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::RendererContext");

	public:

		RendererContext() noexcept = default;
		RendererContext(const RendererContext& other) noexcept = delete;
		RendererContext(RendererContext&& other) noexcept = default;
		~RendererContext() noexcept = default;

		RendererContext& operator = (const RendererContext& other) noexcept = delete;
		RendererContext& operator = (RendererContext&& other) noexcept = default;

		bool create(wd::Window& window);

		void destroy();

		auto& getCurrentDisplayImage() { return displayImages[currentDisplayImageIndex]; }
		auto& getCurrentDisplayImage() const { return displayImages[currentDisplayImageIndex]; }

		size_t getDisplayImagesCount() const noexcept { return displayImages.size(); }

		auto getCurrentDisplayImageIndex() const noexcept { return currentDisplayImageIndex; }

		auto& getNextDisplayImage() { return displayImages[nextDisplayImageIndex]; }
		auto& getNextDisplayImage() const { return displayImages[nextDisplayImageIndex]; }

		uint32_t getNextDisplayImageIndex() noexcept { return nextDisplayImageIndex; }
		uint32_t getNextDisplayImageIndex() const noexcept { return nextDisplayImageIndex; }

		auto& getInstance() noexcept { return instance; }
		auto& getInstance() const noexcept { return instance; }
		auto& getDebugUtilsMessenger() noexcept { return debugUtilsMessenger; }
		auto& getDebugUtilsMessenger() const noexcept { return debugUtilsMessenger; }
		auto& getPhysicalDevice() noexcept { return physicalDevice; }
		auto& getPhysicalDevice() const noexcept { return physicalDevice; }
		auto& getSurface() noexcept { return surface; }
		auto& getSurface() const noexcept { return surface; }
		auto& getDevice() noexcept { return device; }
		auto& getDevice() const noexcept { return device; }
		auto& getVMA() noexcept { return vma; }
		auto& getVMA() const noexcept { return vma; }
		auto& getSwapChain() noexcept { return swapChain; }
		auto& getSwapChain() const noexcept { return swapChain; }
		auto& getQueue() noexcept { return queue; }
		auto& getQueue() const noexcept { return queue; }
		auto& getCommandPool() noexcept { return commandPool; }
		auto& getCommandPool() const noexcept { return commandPool; }
		auto& getRenderPass() noexcept { return renderPass; }
		auto& getRenderPass() const noexcept { return renderPass; }
		auto& getDescriptorPool() noexcept { return descriptorPool; }
		auto& getDescriptorPool() const noexcept { return descriptorPool; }

	protected:

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Surface surface{ nullptr };
		Device device{ nullptr };
		VMA vma{ nullptr };
		SwapChain swapChain{ nullptr };
		Queue queue{ nullptr };
		CommandPool commandPool{ nullptr };

		RenderPass renderPass{ nullptr };

		using DisplayImages = std::vector<DisplayImage>;
		DisplayImages displayImages;
		uint32_t currentDisplayImageIndex = 0;
		uint32_t nextDisplayImageIndex = 0;

		bool createDisplayImages();

		DescriptorPool descriptorPool{ nullptr };

		// Descriptor set for global data (camera, time, global textures etc.)
		DescriptorSetLayout globalDescriptorSetLayout{ nullptr };
		DescriptorSets globalDescriptorSets;

		// TODO: DescriptorSet for render pass?
		//DescriptorSetLayout renderPassDescriptorSetLayout{ nullptr };
		//DescriptorSets renderPassDescriptorSet{ nullptr };

		size_t framesInFlight = 3;

		bool invalidWindowSizeForPresent = false;

		void windowResized(const Vector2i& size);

	};
}