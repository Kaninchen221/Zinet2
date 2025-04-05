#pragma once

#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"

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

		instance.invalidate();
		const VkInstance invalidInstanceAfterInvalidate = instance.get();
		ASSERT_FALSE(invalidInstanceAfterInvalidate);
	}

	TEST(Instance, GetGlfwRequiredInstanceExtensions)
	{
		wd::GLFW::Init();

		std::vector<const char*> extensions = Instance::GetGlfwRequiredInstanceExtensions();
		const std::string_view khr_surface = "VK_KHR_surface"; // We need it for the VkSurface

		// https://github.com/shadps4-emu/shadPS4/issues/799#issuecomment-2391126439
		// "gcc 13 with full C++23 support won't be included until Debian Trixie is released."
#if ZINET_LINUX
		ASSERT_NE(std::find(extensions.begin(), extensions.end(), khr_surface), extensions.end());
#else
		ASSERT_NE(std::ranges::find(extensions, khr_surface), extensions.cend());
#endif

		wd::GLFW::Deinit();
	}
}