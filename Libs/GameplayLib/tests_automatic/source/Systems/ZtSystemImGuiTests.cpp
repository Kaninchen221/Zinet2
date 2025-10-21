#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtGLFW.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::system::tests
{
	class SystemImGuiTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();
		}

		void TearDown() override
		{
			wd::GLFW::Deinit();
		}

	};

	TEST_F(SystemImGuiTests, PassTest)
	{
		// TODO: We need first valid Renderer and RendererContext
	}
}