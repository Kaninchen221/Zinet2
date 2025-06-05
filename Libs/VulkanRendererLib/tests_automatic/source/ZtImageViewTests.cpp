#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>
#include <ranges>

namespace zt::vulkan_renderer::tests
{
	class ImageViewTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();
		}

		void TearDown() override
		{
			wd::GLFW::Deinit();
		}

		ImageView imageView{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<ImageView, VkImageView>());
	};

	TEST_F(ImageViewTests, PassTest)
	{}

	TEST(ImageView, CreateFromSwapChainImageTest)
	{
		wd::GLFW::Init();

		wd::Window window;
		window.create(2, 2);

		Instance instance{ nullptr };
		instance.setEnableValidationLayers(true);
		ASSERT_TRUE(instance.create());

		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		ASSERT_TRUE(debugUtilsMessenger.create(instance));

		auto physicalDevices = instance.getPhysicalDevices();
		PhysicalDevice physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
		invalidateAll(physicalDevices);
		ASSERT_TRUE(physicalDevice.isValid());

		Surface surface{ nullptr };
		ASSERT_TRUE(surface.create(instance, window));

		Device device{ nullptr };
		ASSERT_TRUE(device.create(physicalDevice, surface));

		SwapChain swapChain{ nullptr };
		ASSERT_TRUE(swapChain.create(device, physicalDevice, surface, window.getFramebufferSize()));

		auto images = swapChain.getImages(device);

		std::vector<ImageView> imageViews;
		imageViews.reserve(images.size());
		for (const auto& image : images)
		{
			auto& imageView = imageViews.emplace_back(nullptr);
			const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
			ASSERT_TRUE(imageView.create(device, imageViewCreateInfo));
			ASSERT_TRUE(imageView.isValid());
			ASSERT_EQ(imageView.getFormat(), swapChain.getFormat());

			imageView.destroy(device);
		}

		swapChain.destroy(device);
		device.destroy();
		physicalDevice.invalidate();
		surface.destroy(instance);
		debugUtilsMessenger.destroy(instance);
		instance.destroy();
	}
}