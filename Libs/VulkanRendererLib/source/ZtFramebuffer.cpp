#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"

namespace zt::vulkan_renderer
{

	bool Framebuffer::create(const Device& device, const RenderPass& renderPass, const ImageView& imageView, const Vector2ui& size) noexcept
	{
		auto attachment = imageView.get();

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass.get();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &attachment;
		framebufferInfo.width = size.x;
		framebufferInfo.height = size.y;
		framebufferInfo.layers = 1;

		const auto result = vkCreateFramebuffer(device.get(), &framebufferInfo, nullptr, &objectHandle);
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