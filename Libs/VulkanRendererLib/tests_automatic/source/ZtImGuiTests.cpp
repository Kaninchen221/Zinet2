#pragma once

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class ImGuiTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			window.create(1, 1);
			rendererContext.create(window);

			imGui.init(rendererContext, window);
		}

		void TearDown() override
		{
			imGui.deinit(rendererContext);
			rendererContext.destroy();

			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		RendererContext rendererContext;
		wd::Window window;
		ImGuiIntegration imGui;

		static_assert(std::is_default_constructible_v<ImGuiIntegration>);
		static_assert(!std::is_copy_constructible_v<ImGuiIntegration>);
		static_assert(!std::is_copy_assignable_v<ImGuiIntegration>);
		static_assert(std::is_move_constructible_v<ImGuiIntegration>);
		static_assert(std::is_move_assignable_v<ImGuiIntegration>);
		static_assert(std::is_destructible_v<ImGuiIntegration>);
	};

	TEST_F(ImGuiTests, Test)
	{

	}
}