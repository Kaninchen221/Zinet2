#pragma once

#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include <gtest/gtest.h>

using namespace zt::core;
using namespace zt::vulkan_renderer;

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

		auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
		ASSERT_TRUE(imGuiIntegrationRes);
		ASSERT_TRUE(imGuiIntegrationRes->isInitialized());

		// ImGui should add it's own graphics pipeline to render the UI
		const ecs::Query<GraphicsPipeline> query{ world };
		EXPECT_EQ(query.getComponentsCount(), 1);

		for (const auto [graphicsPipeline] : query)
		{
			ASSERT_TRUE(graphicsPipeline);
			EXPECT_TRUE(graphicsPipeline->isValid());
		}

		ImGui::Deinit(world);
		Renderer::Deinit(world);
		Window::Deinit(world);
	}
}