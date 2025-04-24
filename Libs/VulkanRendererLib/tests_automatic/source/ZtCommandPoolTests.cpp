#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class CommandPoolTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			queue = device.getQueue();
			ASSERT_TRUE(queue.isValid());

			ASSERT_TRUE(commandPool.create(device, queue));
		}

		void TearDown() override
		{
			commandPool.destroy(device);
			ASSERT_FALSE(commandPool.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

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

		static_assert(std::is_base_of_v<VulkanObject<VkCommandPool>, CommandPool>);

		static_assert(std::is_constructible_v<CommandPool, VkCommandPool>);
		static_assert(!std::is_default_constructible_v<CommandPool>);
		static_assert(!std::is_copy_constructible_v<CommandPool>);
		static_assert(!std::is_copy_assignable_v<CommandPool>);
		static_assert(std::is_move_constructible_v<CommandPool>);
		static_assert(std::is_move_assignable_v<CommandPool>);
		static_assert(std::is_destructible_v<CommandPool>);
	};

	TEST_F(CommandPoolTests, PassTest)
	{}
}