#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include <gtest/gtest.h>

#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer::tests
{
	class RenderPassTests : public ::testing::Test
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

			ASSERT_TRUE(device.create(physicalDevice, surface));

			const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(VK_FORMAT_B8G8R8A8_SRGB);
			ASSERT_TRUE(renderPass.create(device, renderPassCreateInfo));
			ASSERT_TRUE(renderPass.isValid());
		}

		void TearDown() override
		{
			renderPass.destroy(device);
			ASSERT_FALSE(renderPass.isValid());

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

		wd::Window window;
		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Surface surface{ nullptr };
		Device device{ nullptr };
		RenderPass renderPass{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<RenderPass, VkRenderPass>());
	};

	TEST_F(RenderPassTests, PassTest)
	{}
}