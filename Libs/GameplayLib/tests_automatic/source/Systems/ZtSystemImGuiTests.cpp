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

		// TODO: ImGuiSystem shouldn't have it's own graphics pipeline because ImGui back end is for that
		// Now the SystemRenderer should just iterate over the RendererDrawData components
		// The component should now just contains a Command of type: " std::function<void(CommandBuffer& commandBuffer)> "
		// Because for now our target is to render only the ImGui on the screen and then we are can start thinking about other like sprites

		ImGui::Deinit(world);
		Renderer::Deinit(world);
		Window::Deinit(world);
	}
}