#pragma once

#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

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
			ASSERT_TRUE(instance.isValid());
		}

		void TearDown() override
		{
			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			wd::GLFW::Deinit();
		}

		Instance instance;
		PhysicalDevice physicalDevice{ nullptr };

		void createPhysicalDevice() 
		{  
			auto physicalDevices = instance.getPhysicalDevices();
			ASSERT_FALSE(physicalDevices.empty());

			physicalDevice = std::move(physicalDevices.front());
			ASSERT_TRUE(physicalDevice.isValid());
		}

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
		createPhysicalDevice();
		const VkPhysicalDeviceProperties properties = physicalDevice.getVkPhysicalDeviceProperties();
	}

	TEST_F(PhysicalDeviceTests, GetVkPhysicalDeviceFeaturesTest)
	{
		createPhysicalDevice();
		const VkPhysicalDeviceFeatures features = physicalDevice.getVkPhysicalDeviceFeatures();
	}

	TEST_F(PhysicalDeviceTests, TakeBestDeviceTest)
	{
		auto physicalDevices = instance.getPhysicalDevices();
		PhysicalDevice bestPhysicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);

		ASSERT_TRUE(bestPhysicalDevice.isValid());
	}

	TEST_F(PhysicalDeviceTests, GetVkQueueFamiliesPropertiesTest)
	{
		createPhysicalDevice();
		const std::vector<VkQueueFamilyProperties> queuesFamiliesProperties = physicalDevice.getVkQueuesFamiliesProperties();
	}

	TEST_F(PhysicalDeviceTests, TakeQueueFamilyIndexForPresentTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(2, 2);

		Surface surface = instance.createSurface(window);
		ASSERT_TRUE(surface.isValid());

		auto queuesFamiliesProperties = physicalDevice.getVkQueuesFamiliesProperties();
		const std::int32_t queueFamilyIndex = physicalDevice.takeQueueFamilyIndexForPresent(queuesFamiliesProperties);
		ASSERT_NE(queueFamilyIndex, PhysicalDevice::InvalidIndex);

		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, TakeQueueFamilyIndexForSurfaceTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(2, 2);

		Surface surface = instance.createSurface(window);
		ASSERT_TRUE(surface.isValid());

		auto queuesFamiliesProperties = physicalDevice.getVkQueuesFamiliesProperties();
		const std::int32_t queueFamilyIndex = physicalDevice.takeQueueFamilyIndexForSurface(queuesFamiliesProperties, surface);
		ASSERT_NE(queueFamilyIndex, PhysicalDevice::InvalidIndex);

		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, CreateDeviceTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(2, 2);

		Surface surface = instance.createSurface(window);
		ASSERT_TRUE(surface.isValid());

		Device device = physicalDevice.createDevice(surface);
		ASSERT_TRUE(device.isValid());

		device.destroy();
		surface.destroy(instance);
	}
}