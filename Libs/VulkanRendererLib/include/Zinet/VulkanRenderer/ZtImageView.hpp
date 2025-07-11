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

		ImageView() ZINET_API_POST = delete;
		ImageView(const ImageView& other) ZINET_API_POST = delete;
		ImageView(ImageView&& other) ZINET_API_POST = default;
		~ImageView() ZINET_API_POST = default;

		ImageView& operator = (const ImageView& other) ZINET_API_POST = delete;
		ImageView& operator = (ImageView&& other) ZINET_API_POST = default;

		static VkImageViewCreateInfo GetDefaultCreateInfo(const VkImage& vkImage, VkFormat format) ZINET_API_POST;

		bool create(const Device& device, const VkImageViewCreateInfo& createInfo) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

		VkFormat getFormat() const ZINET_API_POST { return format; }

	protected:

		VkFormat format = VK_FORMAT_UNDEFINED;

	};
}