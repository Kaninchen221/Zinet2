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

		ImageView(HandleType newObjectHandle, VkFormat newFormat = VK_FORMAT_UNDEFINED)
			: VulkanObject(newObjectHandle), 
			  format{ newFormat }
		{}

		ImageView() noexcept = delete;
		ImageView(const ImageView& other) noexcept = delete;
		ImageView(ImageView&& other) noexcept = default;
		~ImageView() noexcept = default;

		ImageView& operator = (const ImageView& other) noexcept = delete;
		ImageView& operator = (ImageView&& other) noexcept = default;

		static VkImageViewCreateInfo GetDefaultCreateInfo(const VkImage& vkImage, VkFormat format) noexcept;

		bool create(const Device& device, const VkImageViewCreateInfo& createInfo) noexcept;

		void destroy(const Device& device) noexcept;

		VkFormat getFormat() const noexcept { return format; }

	protected:

		VkFormat format = VK_FORMAT_UNDEFINED;

	};
}