#pragma once

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include <gtest/gtest.h>

#include <imgui.h>

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

		struct CallImGuiWindowLabel{};
		static void CallImGui()
		{
			if (::ImGui::Begin("Test"))
			{}

			::ImGui::End();
		}
	};

	TEST_F(ImGuiTests, PassTest)
	{
		ecs::World world;
		ecs::Schedule schedule;

		{ // Init
			schedule.addSystem(Window{}, Window::Init, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Init, ecs::After{ Window{} }, ecs::MainThread{});
			schedule.addSystem(ImGui{}, ImGui::Init, ecs::After{ Window{}, Renderer{} }, ecs::MainThread{});

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			auto vulkanRendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
			ASSERT_TRUE(vulkanRendererRes);
			ASSERT_TRUE(vulkanRendererRes->isInitialized());

			auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
			ASSERT_TRUE(imGuiIntegrationRes);
			ASSERT_TRUE(imGuiIntegrationRes->isInitialized());

			auto query = ecs::Query<RenderCommand>{ world };
			EXPECT_EQ(query.getComponentCount(), 1);
			
			auto imGuiDataRes = world.getResource<ImGuiData>();
			ASSERT_TRUE(imGuiDataRes);
			ASSERT_FALSE(imGuiDataRes->skipImGui);

			schedule.clear();
		}

		{ // Update
			schedule.addSystem(Window{}, Window::Update, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Update, ecs::After{ Window{} }, ecs::MainThread{});

			schedule.addSystem(ImGui::Pre{}, ImGui::PreUpdate, ecs::Before{ Renderer{} }, ecs::After{ Window{} }, ecs::MainThread{});
			schedule.addSystem(ImGui::Post{}, ImGui::PostUpdate, ecs::Before{ Renderer{} }, ecs::After{ ImGui::Pre{} }, ecs::MainThread{});

			schedule.addSystem(CallImGuiWindowLabel{}, CallImGui, ecs::After{ ImGui::Pre{} }, ecs::Before{ ImGui::Post{} });

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();
		}

		{ // Deinit
			schedule.addSystem(Window{}, Window::Deinit, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Deinit, ecs::Before{ Window{} }, ecs::MainThread{});
			schedule.addSystem(ImGui{}, ImGui::Deinit, ecs::Before{ Window{}, Renderer{} }, ecs::MainThread{});

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
			ASSERT_TRUE(imGuiIntegrationRes);
			ASSERT_FALSE(imGuiIntegrationRes->isInitialized());
		}
	}
}