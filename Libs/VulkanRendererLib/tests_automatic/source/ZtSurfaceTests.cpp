#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class SurfaceTests : public ::testing::Test
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

			window.create(2, 2);

			ASSERT_TRUE(surface.create(instance, window));
		}

		void TearDown() override
		{
			surface.destroy(instance);

			device.destroy();
			ASSERT_FALSE(device.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());
		}

		wd::GLFW glfw;
		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		wd::Window window;
		Surface surface{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkSurfaceKHR>, Surface>);

		static_assert(std::is_constructible_v<Surface, VkSurfaceKHR>);
		static_assert(!std::is_default_constructible_v<Surface>);
		static_assert(!std::is_copy_constructible_v<Surface>);
		static_assert(!std::is_copy_assignable_v<Surface>);
		static_assert(std::is_move_constructible_v<Surface>);
		static_assert(std::is_move_assignable_v<Surface>);
		static_assert(std::is_destructible_v<Surface>);
	};

	TEST_F(SurfaceTests, PassTest)
	{}
}