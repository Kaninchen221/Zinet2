#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDisplayImage.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class DisplayImageTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init(false);
			window.create(2, 2);
			rendererContext.create(window);
		}

		void TearDown() override
		{
			rendererContext.destroy();
			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		wd::Window window{};
		RendererContext rendererContext{};
	};

	TEST_F(DisplayImageTests, Test)
	{
		[[maybe_unused]]
		DisplayImage& displayImage = rendererContext.getCurrentDisplayImage();
	}
}