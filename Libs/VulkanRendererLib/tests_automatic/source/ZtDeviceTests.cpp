#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include <type_traits>

#include "ZtVulkanObjectTestsUtils.hpp"

namespace zt::vulkan_renderer::tests
{
	class DeviceTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());
			
			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.createPhysicalDevices();
			physicalDevice = PhysicalDevice::GetBestDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			device = physicalDevice.createDeviceForPresent();
			ASSERT_TRUE(device.isValid());
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

		Instance instance;
		DebugUtilsMessenger debugUtilsMessenger;
		PhysicalDevice physicalDevice{ nullptr };
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

	TEST_F(DeviceTests, Test)
	{
	}
}