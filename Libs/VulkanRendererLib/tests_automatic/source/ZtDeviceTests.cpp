#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class DeviceTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			window.create(2, 2);

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());
			
			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(surface.create(instance, window));

			ASSERT_TRUE(device.create(physicalDevice, surface));
		}

		void TearDown() override
		{
			surface.destroy(instance);
			ASSERT_FALSE(surface.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			window.destroyWindow();

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		wd::Window window;
		Surface surface{ nullptr };
		Device device{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkDevice>, Device>);

		static_assert(std::is_constructible_v<Device, VkDevice>);
		static_assert(!std::is_default_constructible_v<Device>);
		static_assert(!std::is_copy_constructible_v<Device>);
		static_assert(!std::is_copy_assignable_v<Device>);
		static_assert(std::is_move_constructible_v<Device>);
		static_assert(std::is_move_assignable_v<Device>);
		static_assert(std::is_destructible_v<Device>);
	};

	TEST_F(DeviceTests, PassTest)
	{}

	TEST_F(DeviceTests, GetQueueFamilyIndexTest)
	{
		const std::int32_t queueFamilyIndex = device.getQueueFamilyIndex();
		ASSERT_NE(queueFamilyIndex, Device::InvalidIndex);
	}

	TEST_F(DeviceTests, GetQueueTest)
	{
		const Queue presentQueue = device.getQueue();
		ASSERT_TRUE(presentQueue.isValid());
	}
}