#pragma once

#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer::tests
{
	class QueueUtilsTests : public ::testing::Test
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

			queue = device.getQueue();
			ASSERT_TRUE(queue.isValid());

			ASSERT_EQ(queue.getQueueFamilyIndex(), device.getQueueFamilyIndex());

			ASSERT_TRUE(commandPool.create(device, queue));
		}

		void TearDown() override
		{
			commandPool.destroy(device);
			ASSERT_FALSE(commandPool.isValid());

			queue.invalidate();
			ASSERT_FALSE(queue.isValid());

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
		Queue queue{ nullptr };
		CommandPool commandPool{ nullptr };
	};

	TEST_F(QueueUtilsTests, PassTest)
	{
		const auto commands = [&](const CommandBuffer& commandBuffer)
		{
			ASSERT_TRUE(commandBuffer.isValid());
		};

		ASSERT_TRUE(SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, commands));
	}
}