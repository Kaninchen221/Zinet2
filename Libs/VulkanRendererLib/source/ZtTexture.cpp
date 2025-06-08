#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::vulkan_renderer
{

	bool Texture::create(const Device& device, const VMA& vma, const Vector2ui& size) noexcept
	{
		const auto format = VK_FORMAT_R8G8B8A8_SRGB; // Good enough, widespread supported format

		{
			const VkImageCreateInfo createInfo
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.imageType = VK_IMAGE_TYPE_2D,
				.format = format, 
				.extent = VkExtent3D {
					static_cast<uint32_t>(size.x),
					static_cast<uint32_t>(size.y),
					1 },
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 1,
				.pQueueFamilyIndices = &device.getQueueFamilyIndex(),
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
			};

			if (!image.create(vma, createInfo))
			{
				Logger->error("Couldn't create texture");
				return false;
			}
		}

		{
			const auto createInfo = ImageView::GetDefaultCreateInfo(image.get(), format);
			if (!imageView.create(device, createInfo))
			{
				Logger->error("Couldn't create texture");
				return false;
			}
		}

		return true;
	}

	bool Texture::isValid() const noexcept
	{
		return image.isValid() && imageView.isValid();
	}

	void Texture::destroy(const Device& device, const VMA& vma) noexcept
	{
		imageView.destroy(device);
		image.destroy(vma);
	}

	void Texture::fillWithImageBuffer(const FillWithImageBufferInput& input) const noexcept
	{
		const auto& imageOffset = input.imageOffset;
		const auto& imageExtent = input.imageExtent;
		const auto& buffer = input.buffer;
		const auto& commandBuffer = input.commandBuffer;

		{
			const auto imageMemoryBarrier = image.getDefaultMemoryBarier(
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			vkCmdPipelineBarrier(
				commandBuffer.get(),
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier
			);
		}

		const VkBufferImageCopy bufferImageCopy
		{
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,

			.imageSubresource = VkImageSubresourceLayers
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
			.imageOffset = VkOffset3D{ imageOffset.x, imageOffset.y, imageOffset.z },
			.imageExtent =
			{
				imageExtent.x,
				imageExtent.y,
				1
			}
		};

		vkCmdCopyBufferToImage(
			commandBuffer.get(),
			buffer.get(),
			image.get(),
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&bufferImageCopy
		);

		{
			const auto imageMemoryBarrier = image.getDefaultMemoryBarier(
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

			vkCmdPipelineBarrier(
				commandBuffer.get(),
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier
			);
		}
	}

}