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
	class Instance;

	class ZINET_VULKAN_RENDERER_API Framebuffer : public VulkanObject<VkFramebuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRFramebuffer");

	public:

		Framebuffer(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Framebuffer() noexcept = delete;
		Framebuffer(const Framebuffer& other) noexcept = delete;
		Framebuffer(Framebuffer&& other) noexcept = default;
		~Framebuffer() noexcept = default;

		Framebuffer& operator = (const Framebuffer& other) noexcept = delete;
		Framebuffer& operator = (Framebuffer&& other) noexcept = default;

	};
}