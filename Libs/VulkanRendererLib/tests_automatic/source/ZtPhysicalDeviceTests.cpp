#pragma once

#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include <type_traits>

#include "ZtVulkanObjectTestsUtils.hpp"

namespace zt::vulkan_renderer::tests
{
	class PhysicalDeviceTests : public ::testing::Test
	{
	protected:

		PhysicalDeviceTests()
		{
		}

		~PhysicalDeviceTests() override
		{
		}

		void SetUp() override
		{
			wd::GLFW::Init();
			instance.create();

			auto physicalDevices = instance.createPhysicalDevices();
			ASSERT_FALSE(physicalDevices.empty());

			physicalDevice = std::move(physicalDevices.front());
		}

		void TearDown() override
		{
			instance.destroy();
			wd::GLFW::Deinit();
		}

		Instance instance;
		PhysicalDevice physicalDevice{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkPhysicalDevice, false>, PhysicalDevice>);

		static_assert(std::is_constructible_v<PhysicalDevice, VkPhysicalDevice>);
		static_assert(!std::is_default_constructible_v<PhysicalDevice>);
		static_assert(!std::is_copy_constructible_v<PhysicalDevice>);
		static_assert(!std::is_copy_assignable_v<PhysicalDevice>);
		static_assert(std::is_move_constructible_v<PhysicalDevice>);
		static_assert(std::is_move_assignable_v<PhysicalDevice>);
		static_assert(std::is_destructible_v<PhysicalDevice>);
	};

	TEST_F(PhysicalDeviceTests, GetVkPhysicalDevicePropertiesTest)
	{
		const VkPhysicalDeviceProperties properties = physicalDevice.getVkPhysicalDeviceProperties();
	}

	TEST_F(PhysicalDeviceTests, GetVkPhysicalDeviceFeaturesTest)
	{
		const VkPhysicalDeviceFeatures features = physicalDevice.getVkPhysicalDeviceFeatures();
	}

	TEST_F(PhysicalDeviceTests, GetBestDeviceTest)
	{
		auto physicalDevices = instance.createPhysicalDevices();
		PhysicalDevice bestDevice = PhysicalDevice::GetBestDevice(physicalDevices);

		ASSERT_TRUE(bestDevice.isValid());
	}

	TEST_F(PhysicalDeviceTests, GetVkQueueFamiliesPropertiesTest)
	{
		const std::vector<VkQueueFamilyProperties> queueFamiliesProperties = physicalDevice.getVkQueueFamiliesProperties();
	}

	TEST_F(PhysicalDeviceTests, GetQueueFamilyForPresentIndexTest)
	{
		const std::int32_t queueFamilyForPresentIndex = physicalDevice.getQueueFamilyIndexForPresent();
		ASSERT_NE(queueFamilyForPresentIndex, PhysicalDevice::InvalidIndex);
	}
}