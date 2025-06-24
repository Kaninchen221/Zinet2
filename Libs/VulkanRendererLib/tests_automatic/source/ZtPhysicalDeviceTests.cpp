#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

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

			ASSERT_TRUE(instance.create());
		}

		void TearDown() override
		{
			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };

		void createPhysicalDevice() 
		{  
			auto physicalDevices = instance.getPhysicalDevices();
			ASSERT_FALSE(physicalDevices.empty());

			physicalDevice = std::move(physicalDevices.front());
			ASSERT_TRUE(physicalDevice.isValid());

			invalidateAll(physicalDevices);
		}

		static_assert(VulkanObjectDecoratorStaticTest<PhysicalDevice, VkPhysicalDevice>());
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

		invalidateAll(physicalDevices);
		bestPhysicalDevice.invalidate();
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

		Surface surface{ nullptr };
		surface.create(instance, window);
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

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		auto queuesFamiliesProperties = physicalDevice.getVkQueuesFamiliesProperties();
		const std::int32_t queueFamilyIndex = physicalDevice.takeQueueFamilyIndexForSurface(queuesFamiliesProperties, surface);
		ASSERT_NE(queueFamilyIndex, PhysicalDevice::InvalidIndex);

		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, GetDeviceExtensionPropertiesTest)
	{
		createPhysicalDevice();

		const std::vector<VkExtensionProperties> deviceExtensionProperties = physicalDevice.getDeviceExtensionProperties();
		ASSERT_FALSE(deviceExtensionProperties.empty());
	}

	TEST_F(PhysicalDeviceTests, CreateDeviceTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(2, 2);

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		Device device{ nullptr };
		ASSERT_TRUE(device.create(instance, physicalDevice, surface));

		device.destroy();
		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, GetPhysicalDeviceSurfaceCapabilitiesTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(1, 1);

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		const VkSurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getPhysicalDeviceSurfaceCapabilities(surface);

		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, GetPhysicalDeviceSurfaceFormatsTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(1, 1);

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		const std::vector<VkSurfaceFormatKHR> surfaceFormats = physicalDevice.getPhysicalDeviceSurfaceFormats(surface);
		ASSERT_FALSE(surfaceFormats.empty());

		surface.destroy(instance);
	}

	TEST_F(PhysicalDeviceTests, GetPhysicalDeviceSurfacePresentModesTest)
	{
		createPhysicalDevice();

		wd::Window window;
		window.create(1, 1);

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		const std::vector<VkPresentModeKHR> presentModes = physicalDevice.getPhysicalDeviceSurfacePresentModes(surface);
		ASSERT_FALSE(presentModes.empty());

		surface.destroy(instance);
	}
}