#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API ImageView : public VulkanObject<VkImageView>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRImageView");

	public:

		ImageView(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		ImageView() noexcept = delete;
		ImageView(const ImageView& other) noexcept = delete;
		ImageView(ImageView&& other) noexcept = default;
		~ImageView() noexcept = default;

		ImageView& operator = (const ImageView& other) noexcept = delete;
		ImageView& operator = (ImageView&& other) noexcept = default;

		bool create(const VkImage& vkImage, VkFormat format, const Device& device);

		void destroy(const Device& device);

		VkFormat getFormat() const noexcept { return format; }

	protected:

		VkFormat format = VK_FORMAT_UNDEFINED;

	};
}