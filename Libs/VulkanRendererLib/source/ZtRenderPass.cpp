#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	RenderPassCreateInfo RenderPass::GetPresentCreateInfo(VkFormat format) noexcept
	{
		RenderPassCreateInfo createInfo;

		VkAttachmentDescription& colorAttachmentDescription = createInfo.colorAttachmentDescription;
		colorAttachmentDescription.format = format;
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference& colorAttachmentReference = createInfo.colorAttachmentReference;
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription& subpassDescription = createInfo.subpassDescription;
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentReference;

		VkRenderPassCreateInfo& vkCreateInfo = createInfo.vkCreateInfo;
		vkCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		vkCreateInfo.attachmentCount = 1;
		vkCreateInfo.pAttachments = &colorAttachmentDescription;
		vkCreateInfo.subpassCount = 1;
		vkCreateInfo.pSubpasses = &subpassDescription;

		return createInfo;
	}

	RenderPassCreateInfo RenderPass::GetDrawCreateInfo(VkFormat format) noexcept
	{
		RenderPassCreateInfo createInfo;

		VkAttachmentDescription& colorAttachmentDescription = createInfo.colorAttachmentDescription;
		colorAttachmentDescription.format = format;
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_4_BIT;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		VkAttachmentReference& colorAttachmentReference = createInfo.colorAttachmentReference;
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription& subpassDescription = createInfo.subpassDescription;
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentReference;

		VkRenderPassCreateInfo& vkCreateInfo = createInfo.vkCreateInfo;
		vkCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		vkCreateInfo.attachmentCount = 1;
		vkCreateInfo.pAttachments = &colorAttachmentDescription;
		vkCreateInfo.subpassCount = 1;
		vkCreateInfo.pSubpasses = &subpassDescription;

		return createInfo;
	}

	bool RenderPass::create(const Device& device, const RenderPassCreateInfo& createInfo) noexcept
	{
		if (isValid())
			return false;

		const auto result = vkCreateRenderPass(device.get(), &createInfo.vkCreateInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create RenderPass, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void RenderPass::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyRenderPass(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}