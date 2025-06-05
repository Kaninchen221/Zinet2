#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

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

		static_assert(VulkanObjectDecoratorStaticTest<DebugUtilsMessenger, VkDebugUtilsMessengerEXT>());
	};

	TEST_F(DebugUtilsMessengerTests, PassTest)
	{}

}