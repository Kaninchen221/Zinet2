#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>
#include <ranges>

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::vulkan_renderer::tests
{
	class ImageTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			const auto imageCreateInfo = Image::GetDefaultCreateInfo(device);
			ASSERT_TRUE(image.create(vma, imageCreateInfo));
			ASSERT_TRUE(image.isValid());
		}

		void TearDown() override
		{
			image.destroy(vma);
			ASSERT_FALSE(image.isValid());

			vma.destroy();
			ASSERT_FALSE(vma.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		VMA vma{ nullptr };
		Image image{ nullptr, VK_FORMAT_UNDEFINED };

		static_assert(VulkanObjectDecoratorStaticTest<Image, VkImage>());
	};

	TEST_F(ImageTests, PassTest)
	{
		ASSERT_NE(image.getFormat(), VK_FORMAT_UNDEFINED);
	}

	TEST(Image, ImageAsTextureTest)
	{
		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));

		RendererContext rendererContext;
		ASSERT_TRUE(rendererContext.create(window));

		auto& device = rendererContext.device;
		auto& vma = rendererContext.vma;
		auto& queue = rendererContext.queue;
		auto& commandPool = rendererContext.commandPool;

		core::Image sourceImage;

		const std::filesystem::path testFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";
		ASSERT_TRUE(sourceImage.loadFromFile(imagePath, 4));

		VkImageCreateInfo imageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_SRGB, // Good enough, widespread supported format
			.extent = VkExtent3D { 
				static_cast<uint32_t>(sourceImage.getWidth()), 
				static_cast<uint32_t>(sourceImage.getHeight()), 
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

		Image image{ nullptr };
		ASSERT_TRUE(image.create(vma, imageCreateInfo));

		// Refactor
		const VkBufferCreateInfo bufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = static_cast<VkDeviceSize>(sourceImage.getSize()),
			.usage = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
		Buffer buffer{ nullptr };
		ASSERT_TRUE(buffer.createBuffer(bufferCreateInfo, vma));

		ASSERT_TRUE(buffer.fillWithImage(sourceImage, vma));

		{ // Layout transition
			const auto layoutTransitionCommands = [&image = image](const CommandBuffer& commandBuffer) -> void
			{
				const auto imageMemoryBarrier = image.getDefaultMemoryBarier();
				vkCmdPipelineBarrier(
					commandBuffer.get(),
					VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier
				);
			};
			ASSERT_TRUE(SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, layoutTransitionCommands));
		} 

		{ // Copy buffer to image
			const auto bufferImageCopyCommands = [&sourceImage = sourceImage, &buffer = buffer, &image = image](const CommandBuffer& commandBuffer) -> void
			{
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
					.imageOffset = VkOffset3D{ 0, 0, 0 },
					.imageExtent =
					{
						static_cast<uint32_t>(sourceImage.getWidth()),
						static_cast<uint32_t>(sourceImage.getHeight()),
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
			};
			ASSERT_TRUE(SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, bufferImageCopyCommands));
		}

		ImageView imageView{ nullptr };
		{
			const auto createInfo = ImageView::GetDefaultCreateInfo(image.get(), VK_FORMAT_R8G8B8A8_SRGB);
			ASSERT_TRUE(imageView.create(device, createInfo));
		}

		Sampler sampler{ nullptr };
		{
			const auto createInfo = Sampler::GetDefaultCreateInfo();
			ASSERT_TRUE(sampler.create(device, createInfo));
		}

		sampler.destroy(device);
		imageView.destroy(device);
		buffer.destroy(vma);
		image.destroy(vma);
		rendererContext.destroy();

		wd::GLFW::Deinit();
	}
}