#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer::tests
{
	class VulkanRendererTests : public ::testing::Test
	{
	protected:

		VulkanRendererTests()
		{
		}

		~VulkanRendererTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		VulkanRenderer renderer;
	};

	TEST_F(VulkanRendererTests, Test)
	{
		wd::GLFW::Init();

		wd::Window window;
		window.create(2, 2);

		renderer.start(window);

		while (window.isOpen())
		{
			renderer.draw(); // For now empty because we store draw data in shaders

			window.requestCloseWindow();
		}

		renderer.shutdown();

		window.destroyWindow();

		wd::GLFW::Deinit();
	}
}