#pragma once

#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>
#include <ranges>

namespace zt::vulkan_renderer::tests
{
	class ImageTests : public ::testing::Test
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

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			const auto imageCreateInfo = Image::GetDefaultCreateInfo(device);
			ASSERT_TRUE(image.create(vma, imageCreateInfo));
			ASSERT_TRUE(image.isValid());
		}

		void TearDown() override
		{
			image.destroy(vma);
			ASSERT_FALSE(image.isValid());

			vma.destroy();
			ASSERT_FALSE(vma.isValid());

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
		VMA vma{ nullptr };
		Image image{ nullptr, VK_FORMAT_UNDEFINED };

		static_assert(std::is_base_of_v<VulkanObject<VkImage>, Image>);

		static_assert(std::is_constructible_v<Image, VkImage>);
		static_assert(!std::is_default_constructible_v<Image>);
		static_assert(!std::is_copy_constructible_v<Image>);
		static_assert(!std::is_copy_assignable_v<Image>);
		static_assert(std::is_move_constructible_v<Image>);
		static_assert(std::is_move_assignable_v<Image>);
		static_assert(std::is_destructible_v<Image>);
	};

	TEST_F(ImageTests, PassTest)
	{
		ASSERT_NE(image.getFormat(), VK_FORMAT_UNDEFINED);
	}
}