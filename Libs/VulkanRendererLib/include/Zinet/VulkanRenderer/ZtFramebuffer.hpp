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

	class  Framebuffer : public VulkanObject<VkFramebuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRFramebuffer");

	public:

		Framebuffer(HandleType newObjectHandle) noexcept
			: VulkanObject(newObjectHandle) {}

		Framebuffer() noexcept = delete;
		Framebuffer(const Framebuffer& other) noexcept = delete;
		Framebuffer(Framebuffer&& other) noexcept = default;
		~Framebuffer() noexcept = default;

		Framebuffer& operator = (const Framebuffer& other) noexcept = delete;
		Framebuffer& operator = (Framebuffer&& other) noexcept = default;

		bool create(const Device& device, const RenderPass& renderPass, const ImageView& imageView, const Vector2ui& size);

		void destroy(const Device& device) noexcept;

	protected:

		VkFormat format = VK_FORMAT_UNDEFINED;
	};
}