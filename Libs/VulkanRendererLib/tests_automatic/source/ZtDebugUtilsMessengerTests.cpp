#pragma once

#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include "ZtVulkanObjectTestsUtils.hpp"

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
			instance.create();
		}

		void TearDown() override
		{
			instance.destroy();
			wd::GLFW::Deinit();
		}

		Instance instance;
		DebugUtilsMessenger debugUtilsMessenger;

		static_assert(std::is_base_of_v<VulkanObject<VkDebugUtilsMessengerEXT>, DebugUtilsMessenger>);

		static_assert(std::is_default_constructible_v<DebugUtilsMessenger>);
		static_assert(!std::is_copy_constructible_v<DebugUtilsMessenger>);
		static_assert(!std::is_copy_assignable_v<DebugUtilsMessenger>);
		static_assert(std::is_move_constructible_v<DebugUtilsMessenger>);
		static_assert(std::is_move_assignable_v<DebugUtilsMessenger>);
		static_assert(std::is_destructible_v<DebugUtilsMessenger>);
	};

	TEST_F(DebugUtilsMessengerTests, CreateDestroyTest)
	{
		const auto createFunc = [&]() -> bool { return debugUtilsMessenger.create(instance); };
		const auto destroyFunc = [&]() { debugUtilsMessenger.destroy(instance); };

		VulkanObjectTestsUtils::TestCreationDestruction(debugUtilsMessenger, createFunc, destroyFunc);
	}

}