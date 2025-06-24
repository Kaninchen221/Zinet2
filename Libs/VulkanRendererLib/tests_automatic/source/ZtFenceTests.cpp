#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class FenceTests : public ::testing::Test
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

			ASSERT_TRUE(fence.create(device));
			ASSERT_TRUE(fence.isValid());
		}

		void TearDown() override
		{
			fence.destroy(device);
			ASSERT_FALSE(fence.isValid());

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
		Fence fence{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Fence, VkFence>());
	};

	TEST_F(FenceTests, PassTest)
	{
		ASSERT_TRUE(fence.isSignaled(device));
		ASSERT_TRUE(fence.wait(device));
		ASSERT_TRUE(fence.reset(device));
		ASSERT_FALSE(fence.isSignaled(device));

	}
}