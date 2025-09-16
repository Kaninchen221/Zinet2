#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"

#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer::tests
{
	class TextureTests : public ::testing::Test
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

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			queue = device.getQueue();
			ASSERT_TRUE(queue.isValid());

			ASSERT_TRUE(commandPool.create(device, queue));
		}

		void TearDown() override
		{
			commandPool.destroy(device);
			ASSERT_FALSE(commandPool.isValid());

			queue.invalidate();
			ASSERT_FALSE(queue.isValid());

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
		Queue queue{ nullptr };
		CommandPool commandPool{ nullptr };
		Texture texture;

		static_assert(std::is_default_constructible_v<Texture>);
		static_assert(!std::is_copy_constructible_v<Texture>);
		static_assert(!std::is_copy_assignable_v<Texture>);
		static_assert(std::is_move_constructible_v<Texture>);
		static_assert(std::is_move_assignable_v<Texture>);
		static_assert(std::is_destructible_v<Texture>);
	};

	TEST_F(TextureTests, PassTest)
	{
		ASSERT_FALSE(texture.isValid());

		ASSERT_TRUE(texture.create(device, vma, { 1, 1 }));
		ASSERT_TRUE(texture.isValid());
		ASSERT_TRUE(texture.getImage().isValid());
		ASSERT_TRUE(texture.getImageView().isValid());

		texture.destroy(device, vma);

		ASSERT_FALSE(texture.isValid());
	}

	TEST_F(TextureTests, FillWithBufferTest)
	{
		const std::filesystem::path testFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";

		core::Image sourceImage;
		ASSERT_TRUE(sourceImage.loadFromFile(imagePath, 4));
		
		Buffer buffer{ nullptr };
		const auto bufferCreateInfo = Buffer::GetImageBufferCreateInfo(sourceImage);
		ASSERT_TRUE(buffer.createBuffer(vma, bufferCreateInfo));

		ASSERT_TRUE(buffer.fillWithImage(vma, sourceImage));

		ASSERT_TRUE(texture.create(device, vma, { sourceImage.getWidth(), sourceImage.getHeight() }));
		ASSERT_TRUE(texture.isValid());

		const auto commands = [&texture = texture, &sourceImage = sourceImage, &buffer = buffer](const CommandBuffer& commandBuffer)
		{
			FillWithImageBufferInput input
			{
				.buffer = buffer,
				.commandBuffer = commandBuffer,
				.imageExtent = { sourceImage.getWidth(), sourceImage.getHeight() }
			};
			texture.fillWithImageBuffer(input);
		};
		SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, commands);

		buffer.destroy(vma);
		texture.destroy(device, vma);
	}
}