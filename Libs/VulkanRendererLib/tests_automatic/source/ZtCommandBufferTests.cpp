#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class CommandBufferTests : public ::testing::Test
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

			ASSERT_TRUE(device.create(instance, physicalDevice, Surface{ nullptr }));

			queue = device.getQueue();
			ASSERT_TRUE(queue.isValid());

			ASSERT_TRUE(commandPool.create(device, queue));

			ASSERT_TRUE(commandBuffer.create(device, commandPool));
			ASSERT_TRUE(commandBuffer.isValid());
		}

		void TearDown() override
		{
			commandPool.destroy(device);
			ASSERT_FALSE(commandPool.isValid());

			commandBuffer.invalidate();
			ASSERT_FALSE(commandBuffer.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			queue.invalidate();
			ASSERT_FALSE(queue.isValid());

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
		Queue queue{ nullptr };
		CommandPool commandPool{ nullptr };
		CommandBuffer commandBuffer{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<CommandBuffer, VkCommandBuffer>());
	};

	TEST_F(CommandBufferTests, RenderPassTest)
	{
		// Prepare render pass and framebuffer
		VMA vma{ nullptr };
		vma.create(device, physicalDevice, instance);
		ASSERT_TRUE(vma.isValid());

		Image image{ nullptr, VK_FORMAT_UNDEFINED };
		auto imageCreateInfo = Image::GetDefaultCreateInfo(device);
		imageCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_4_BIT;
		image.create(vma, imageCreateInfo);
		ASSERT_TRUE(image.isValid());

		RenderPass renderPass{ nullptr };
		const auto renderPassCreateInfo = RenderPass::GetDrawCreateInfo(image.getFormat());
		renderPass.create(device, renderPassCreateInfo);
		ASSERT_TRUE(renderPass.isValid());

		ImageView imageView{ nullptr };
		const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image.get(), image.getFormat());
		imageView.create(device, imageViewCreateInfo);
		ASSERT_TRUE(imageView.isValid());

		Framebuffer framebuffer{ nullptr };
		framebuffer.create(device, renderPass, imageView, { 1, 1 });
		ASSERT_TRUE(framebuffer.isValid());
		//

		ASSERT_TRUE(commandBuffer.begin());

		commandBuffer.beginRenderPass(renderPass, framebuffer, { 1, 1 });
		
		//vkCmdDraw(commandBuffer.get(), 3, 1, 0, 0);
		
		commandBuffer.endRenderPass();

		ASSERT_TRUE(commandBuffer.end());

		// Release render pass, framebuffer and other resources
		framebuffer.destroy(device);
		imageView.destroy(device);
		image.destroy(vma);
		vma.destroy();
		renderPass.destroy(device);
	}

	TEST_F(CommandBufferTests, DestroyTest)
	{
		commandBuffer.destroy(device, commandPool);
		ASSERT_FALSE(commandBuffer.isValid());
	}

	TEST_F(CommandBufferTests, GetDefaultSubmitInfo)
	{
		[[maybe_unused]] const VkSubmitInfo submitInfo = CommandBuffer::GetDefaultSubmitInfo();
	}
}