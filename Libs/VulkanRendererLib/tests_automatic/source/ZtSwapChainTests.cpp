#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

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
	class SwapChainTests : public ::testing::Test
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
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(surface.create(instance, window));

			ASSERT_TRUE(device.create(instance, physicalDevice, surface));

			ASSERT_TRUE(swapChain.create(device, physicalDevice, surface, window.getFramebufferSize()));
		}

		void TearDown() override
		{
			swapChain.destroy(device);
			ASSERT_FALSE(swapChain.isValid());

			surface.destroy(instance);
			ASSERT_FALSE(surface.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

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
		SwapChain swapChain{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<SwapChain, VkSwapchainKHR>());
	};

	TEST_F(SwapChainTests, PassTest)
	{
		EXPECT_NE(swapChain.getFormat(), VkFormat::VK_FORMAT_UNDEFINED);
		EXPECT_NE(swapChain.getExtent().width, VkExtent2D{}.width);
		EXPECT_NE(swapChain.getExtent().height, VkExtent2D{}.height);

		std::vector<VkImage> swapChainImages = swapChain.getImages(device);
		ASSERT_FALSE(swapChainImages.empty());

		swapChain.destroy(device);
	}
}