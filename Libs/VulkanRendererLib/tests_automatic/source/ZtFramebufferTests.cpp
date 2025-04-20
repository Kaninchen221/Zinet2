#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtImage.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class FramebufferTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			// TODO: Complete it after integrating VMA

			//const auto imageCreateInfo = Image::GetDefaultCreateInfo(device);
			//ASSERT_TRUE(image.create(device, imageCreateInfo));

			//ASSERT_TRUE(imageView.create(image.get(), image.getFormat(), device));
		}

		void TearDown() override
		{
			//imageView.destroy(device);
			//ASSERT_FALSE(imageView.isValid());

			//image.destroy(device);
			//ASSERT_FALSE(image.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		Image image{ nullptr };
		ImageView imageView{ nullptr };
		Framebuffer framebuffer{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkFramebuffer>, Framebuffer>);

		static_assert(std::is_constructible_v<Framebuffer, VkFramebuffer>);
		static_assert(!std::is_default_constructible_v<Framebuffer>);
		static_assert(!std::is_copy_constructible_v<Framebuffer>);
		static_assert(!std::is_copy_assignable_v<Framebuffer>);
		static_assert(std::is_move_constructible_v<Framebuffer>);
		static_assert(std::is_move_assignable_v<Framebuffer>);
		static_assert(std::is_destructible_v<Framebuffer>);
	};

	TEST_F(FramebufferTests, PassTest)
	{}
}