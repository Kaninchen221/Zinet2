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

		SwapChain() noexcept = delete;
		SwapChain(const SwapChain& other) noexcept = delete;
		SwapChain(SwapChain&& other) noexcept = default;
		~SwapChain() noexcept = default;

		SwapChain& operator = (const SwapChain& other) noexcept = delete;
		SwapChain& operator = (SwapChain&& other) noexcept = default;

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Surface& surface, const Vector2i& windowFramebufferSize) noexcept;

		void destroy(const Device& device) noexcept;

		std::vector<VkImage> getImages(const Device& device) const noexcept;

		VkFormat getFormat() const noexcept { return format; }

		VkExtent2D getExtent() const noexcept { return extent; }

		std::uint32_t acquireNextImage(const Device& device, Semaphore& semaphore) const noexcept;

	protected:

		VkFormat format{};
		VkExtent2D extent{};

	};
}