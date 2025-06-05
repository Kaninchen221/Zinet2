#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"

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
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			const auto imageCreateInfo = Image::GetDefaultCreateInfo(device);
			ASSERT_TRUE(image.create(vma, imageCreateInfo));

			const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image.get(), image.getFormat());
			ASSERT_TRUE(imageView.create(device, imageViewCreateInfo));

			const auto renderPassCreateInfo = RenderPass::GetDrawCreateInfo(image.getFormat());
			ASSERT_TRUE(renderPass.create(device, renderPassCreateInfo));

			ASSERT_TRUE(framebuffer.create(device, renderPass, imageView, { 1, 1 }));
			ASSERT_TRUE(framebuffer.isValid());
		}

		void TearDown() override
		{
			framebuffer.destroy(device);
			ASSERT_FALSE(framebuffer.isValid());

			renderPass.destroy(device);
			ASSERT_FALSE(renderPass.isValid());

			imageView.destroy(device);
			ASSERT_FALSE(imageView.isValid());

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
		RenderPass renderPass{ nullptr };
		Image image{ nullptr };
		ImageView imageView{ nullptr };
		Framebuffer framebuffer{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Framebuffer, VkFramebuffer>());
	};

	TEST_F(FramebufferTests, PassTest)
	{}
}