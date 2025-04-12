#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

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

		void destroy(Device& device) noexcept;

		std::vector<VkImage> getImages(Device& device) noexcept;

	protected:


	};
}