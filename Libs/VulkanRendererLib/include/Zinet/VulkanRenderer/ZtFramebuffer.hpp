#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Device;
	class RenderPass;
	class ImageView;

	class ZINET_VULKAN_RENDERER_API Framebuffer : public VulkanObject<VkFramebuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRFramebuffer");

	public:

		Framebuffer(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Framebuffer() ZINET_API_POST = delete;
		Framebuffer(const Framebuffer& other) ZINET_API_POST = delete;
		Framebuffer(Framebuffer&& other) ZINET_API_POST = default;
		~Framebuffer() ZINET_API_POST = default;

		Framebuffer& operator = (const Framebuffer& other) ZINET_API_POST = delete;
		Framebuffer& operator = (Framebuffer&& other) ZINET_API_POST = default;

		bool create(const Device& device, const RenderPass& renderPass, const ImageView& imageView, const Vector2ui& size) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	protected:

		VkFormat format = VK_FORMAT_UNDEFINED;
	};
}