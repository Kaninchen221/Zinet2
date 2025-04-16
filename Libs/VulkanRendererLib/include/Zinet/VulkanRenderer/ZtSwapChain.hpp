#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

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

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Surface& surface, const wd::Window& window) noexcept;

		void destroy(const Device& device) noexcept;

		std::vector<VkImage> getImages(Device& device) noexcept;

		VkFormat getFormat() const noexcept { return format; }

	protected:

		VkFormat format{};

	};
}