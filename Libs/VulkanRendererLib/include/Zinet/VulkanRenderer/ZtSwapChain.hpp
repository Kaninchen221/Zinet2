#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <vulkan/vulkan.h>

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Device;
	class PhysicalDevice;
	class Surface;
	class Semaphore;

	class ZINET_VULKAN_RENDERER_API SwapChain : public VulkanObject<VkSwapchainKHR>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSwapChain");

	public:

		SwapChain(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		SwapChain() ZINET_API_POST = delete;
		SwapChain(const SwapChain& other) ZINET_API_POST = delete;
		SwapChain(SwapChain&& other) ZINET_API_POST = default;
		~SwapChain() ZINET_API_POST = default;

		SwapChain& operator = (const SwapChain& other) ZINET_API_POST = delete;
		SwapChain& operator = (SwapChain&& other) ZINET_API_POST = default;

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Surface& surface, const Vector2i& windowFramebufferSize) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

		std::vector<VkImage> getImages(const Device& device) const ZINET_API_POST;

		VkFormat getFormat() const ZINET_API_POST { return format; }

		VkExtent2D getExtent() const ZINET_API_POST { return extent; }

		std::uint32_t acquireNextImage(const Device& device, Semaphore& semaphore) const ZINET_API_POST;

	protected:

		VkFormat format{};
		VkExtent2D extent{};

	};
}