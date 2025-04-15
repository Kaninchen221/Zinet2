#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class QueueTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr })); // We don't need valid Surface for Queue tests

			queue = device.getQueue();
			ASSERT_TRUE(queue.isValid());
		}

		void TearDown() override
		{
			device.destroy();
			ASSERT_FALSE(device.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		Queue queue{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkQueue, false>, Queue>);

		static_assert(std::is_constructible_v<Queue, VkQueue>);
		static_assert(!std::is_default_constructible_v<Queue>);
		static_assert(!std::is_copy_constructible_v<Queue>);
		static_assert(!std::is_copy_assignable_v<Queue>);
		static_assert(std::is_move_constructible_v<Queue>);
		static_assert(std::is_move_assignable_v<Queue>);
		static_assert(std::is_destructible_v<Queue>);
	};

	TEST_F(QueueTests, PassTest)
	{}
}