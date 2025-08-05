#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class InstanceTests : public ::testing::Test
	{
	protected:

		InstanceTests()
		{
		}

		~InstanceTests() override
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

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Instance, VkInstance>());
	};

	TEST_F(InstanceTests, PassTest)
	{}

	TEST_F(InstanceTests, GetRequiredExtensionsTest)
	{
		std::vector<const char*> requiredExtensions = instance.getRequiredExtensions();
		ASSERT_TRUE(core::Contains(requiredExtensions, std::string_view(VK_KHR_SURFACE_EXTENSION_NAME)));
		ASSERT_TRUE(core::Contains(requiredExtensions, std::string_view(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)));

		instance.setEnableValidationLayers(false);
		ASSERT_FALSE(instance.getEnableValidationLayers());

		requiredExtensions = instance.getRequiredExtensions();
		ASSERT_FALSE(core::Contains(requiredExtensions, std::string_view(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)));
	}

	TEST(Instance, GetGlfwRequiredInstanceExtensionsTest)
	{
		wd::GLFW::Init();

		std::vector<const char*> extensions = Instance::GetGlfwRequiredInstanceExtensions();
		const std::string_view khr_surface = VK_KHR_SURFACE_EXTENSION_NAME; // We need it for the VkSurface

		core::Contains(extensions, khr_surface);

		wd::GLFW::Deinit();
	}

	TEST(Instance, GetEnabledLayerNamesTest)
	{
		Instance instance{ nullptr };

		instance.setEnableValidationLayers(false);
		ASSERT_TRUE(instance.getEnabledLayerNames().empty());

		instance.setEnableValidationLayers(true);
		std::vector<const char*> enabledLayerNames = instance.getEnabledLayerNames();
		ASSERT_TRUE(core::Contains(enabledLayerNames, std::string_view("VK_LAYER_KHRONOS_validation")));

		ASSERT_TRUE(instance.areEnabledLayersSupported());

	}

	TEST_F(InstanceTests, GetPhysicalDevicesTest)
	{
		std::vector<PhysicalDevice> physicalDevices = instance.getPhysicalDevices();
		invalidateAll(physicalDevices);
	}
}