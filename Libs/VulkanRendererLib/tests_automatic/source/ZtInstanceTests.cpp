#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include "Zinet/Core/ZtUtils.hpp"

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
		}

		void TearDown() override
		{
			wd::GLFW::Deinit();
		}

		Instance instance;

		static_assert(std::is_default_constructible_v<Instance>);
		static_assert(!std::is_copy_constructible_v<Instance>);
		static_assert(!std::is_copy_assignable_v<Instance>);
		static_assert(std::is_move_constructible_v<Instance>);
		static_assert(std::is_move_assignable_v<Instance>);
		static_assert(std::is_destructible_v<Instance>);
	};

	TEST_F(InstanceTests, Test)
	{
		const VkInstance invalidInstance = instance.get();
		ASSERT_FALSE(invalidInstance);
		ASSERT_FALSE(instance.isValid());

		ASSERT_TRUE(instance.create());

		const VkInstance validInstance = instance.get();
		ASSERT_TRUE(validInstance);
		ASSERT_TRUE(instance.isValid());

		instance.destroy();
		const VkInstance invalidInstanceAfterInvalidate = instance.get();
		ASSERT_FALSE(invalidInstanceAfterInvalidate);
	}

	TEST_F(InstanceTests, GetRequiredExtensionsTest)
	{
		std::vector<const char*> requiredExtensions = instance.getRequiredExtensions();
		ASSERT_TRUE(core::Contains(requiredExtensions, std::string_view(VK_KHR_SURFACE_EXTENSION_NAME)));
		ASSERT_FALSE(core::Contains(requiredExtensions, std::string_view(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)));

		instance.setEnableValidationLayers(true);
		ASSERT_TRUE(instance.getEnableValidationLayers());

		requiredExtensions = instance.getRequiredExtensions();
		ASSERT_TRUE(core::Contains(requiredExtensions, std::string_view(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)));
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
		Instance instance;

		ASSERT_TRUE(instance.getEnabledLayerNames().empty());

		instance.setEnableValidationLayers(true);
		std::vector<const char*> enabledLayerNames = instance.getEnabledLayerNames();
		ASSERT_TRUE(core::Contains(enabledLayerNames, std::string_view("VK_LAYER_KHRONOS_validation")));

		ASSERT_TRUE(instance.areEnabledLayersSupported());

	}
}