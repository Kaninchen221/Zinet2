#pragma once

#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

namespace zt::vulkan_renderer::tests
{
	class DebugUtilsMessengerTests : public ::testing::Test
	{
	protected:

		DebugUtilsMessengerTests()
		{
		}

		~DebugUtilsMessengerTests() override
		{
		}

		void SetUp() override
		{
			wd::GLFW::Init();

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));
		}

		void TearDown() override
		{
			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkDebugUtilsMessengerEXT>, DebugUtilsMessenger>);

		static_assert(!std::is_default_constructible_v<DebugUtilsMessenger>);
		static_assert(!std::is_copy_constructible_v<DebugUtilsMessenger>);
		static_assert(!std::is_copy_assignable_v<DebugUtilsMessenger>);
		static_assert(std::is_move_constructible_v<DebugUtilsMessenger>);
		static_assert(std::is_move_assignable_v<DebugUtilsMessenger>);
		static_assert(std::is_destructible_v<DebugUtilsMessenger>);
	};

	TEST_F(DebugUtilsMessengerTests, PassTest)
	{}

}