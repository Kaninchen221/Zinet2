#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Gameplay/ZtRenderCommand.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

using namespace zt::core;

namespace zt::gameplay::system::tests
{
	class RendererTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			world.addResource(vulkan_renderer::ResourceStorage{});
		}

		void TearDown() override
		{
		}

		ecs::World world;
		ecs::Schedule schedule;
	};

	TEST_F(RendererTests, Test)
	{
		{ // Init
			schedule.addSystem(Window{}, Window::Init, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Init, ecs::After{ Window{} }, ecs::MainThread{});

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			auto vulkanRendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
			ASSERT_TRUE(vulkanRendererRes);
			ASSERT_TRUE(vulkanRendererRes->isInitialized());

			schedule.clear();
		}

		{ // Update
			schedule.addSystem(Window{}, Window::Update, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Update, ecs::After{ Window{} }, ecs::MainThread{});

			bool commandInvoked = false;
			auto command = [&commandInvoked = commandInvoked]
			(vulkan_renderer::CommandBuffer&, const vulkan_renderer::RendererContext&)
			{
				commandInvoked = true;
			};
			RenderCommand renderDrawData{ .shouldDraw = true, .command = command };
			world.spawn(renderDrawData);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			EXPECT_TRUE(commandInvoked);

			schedule.clear();
		}

		{ // Deinit
			schedule.addSystem(Window{}, Window::Deinit, ecs::MainThread{});
			schedule.addSystem(Renderer{}, Renderer::Deinit, ecs::Before{ Window{} }, ecs::MainThread{});

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);
		}
	}
}