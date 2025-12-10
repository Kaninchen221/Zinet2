#pragma once

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class RendererContextTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			window.create(1, 1);
			ASSERT_TRUE(rendererContext.create(window));
		}

		void TearDown() override
		{
			rendererContext.destroy();
			window.destroyWindow();

			wd::GLFW::Deinit();
		}

		static_assert(std::is_default_constructible_v<RendererContext>);
		static_assert(!std::is_copy_constructible_v<RendererContext>);
		static_assert(!std::is_copy_assignable_v<RendererContext>);
		static_assert(std::is_move_constructible_v<RendererContext>);
		static_assert(std::is_move_assignable_v<RendererContext>);
		static_assert(std::is_destructible_v<RendererContext>);

		wd::Window window;
		RendererContext rendererContext;
	};

	TEST_F(RendererContextTests, PassTest)
	{
		EXPECT_TRUE(rendererContext.getTransferCommandBuffer());
	}
}