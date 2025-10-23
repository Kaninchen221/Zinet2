#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include <gtest/gtest.h>

using namespace zt::core;

namespace zt::gameplay::system::tests
{
	class ImGuiTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{

		}

		void TearDown() override
		{

		}

	};

	TEST_F(ImGuiTests, PassTest)
	{
		ecs::World world;

		Window::Init(world);
		Renderer::Init(world);
		ImGui::Init(world);

		auto imGuiIntegrationRes = world.getResource<vulkan_renderer::ImGuiIntegration>();
		ASSERT_TRUE(imGuiIntegrationRes);
		ASSERT_TRUE(imGuiIntegrationRes->isInitialized());

		ImGui::Deinit(world);
		Renderer::Deinit(world);
		Window::Deinit(world);
	}
}