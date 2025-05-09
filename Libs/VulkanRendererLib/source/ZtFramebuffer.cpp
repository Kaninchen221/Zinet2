#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"

namespace zt::vulkan_renderer
{

	bool Framebuffer::create(const Device& device, const RenderPass& renderPass, const ImageView& imageView, const Vector2ui& size) noexcept
	{
		if (isValid())
			return false;

		const auto attachment = imageView.get();

		const VkFramebufferCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.renderPass = renderPass.get(),
			.attachmentCount = 1,
			.pAttachments = &attachment,
			.width = size.x,
			.height = size.y,
			.layers = 1
		};

		const auto result = vkCreateFramebuffer(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Can't create swap chain, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Framebuffer::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyFramebuffer(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}