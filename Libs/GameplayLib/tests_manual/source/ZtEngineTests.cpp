#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtTimeLog.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Editor/ZtEditor.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

using namespace zt::gameplay::system;

namespace zt::gameplay::tests
{
	using namespace zt::core;

	// TODO:
	// - Draw ImGui
	//	- Write Editor System
	// - Draw Sprites
	// - Draw Flipbooks
	// 
	// - Remove nodes
	// - Remove old systems
	// - Refactor
	
	class EngineTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::gameplay::tests::EngineTests");

		EngineTests()
		{
		}

		~EngineTests() override
		{
		}

		void SetUp() override
		{
			ZT_TIME_LOG(addResources());

			ZT_TIME_LOG(init());
		}

		void addResources()
		{
			AssetStorage assetStorage;
			assetStorage.registerAssetClass<core::asset::Text>();
			assetStorage.registerAssetClass<asset::Texture>();
			assetStorage.registerAssetClass<asset::Shader>();
			assetStorage.registerAssetClass<asset::Sampler>();
			ZT_TIME_LOG(assetStorage.storeAssets());

			world.addResource(assetStorage);
		}

		void init()
		{
			scheduleInit.addSystem(Window{}, Window::Init, ecs::MainThread{});
			scheduleInit.addSystem(Renderer{}, Renderer::Init, ecs::After{ Window{} }, ecs::MainThread{});
			scheduleInit.addSystem(ImGui{}, ImGui::Init, ecs::After{ Window{}, Renderer{} }, ecs::MainThread{});
			scheduleInit.addSystem(Sprites{}, Sprites::Init, ecs::After{ Renderer{} });

			scheduleInit.buildGraph();
			scheduleInit.resolveGraph();
			scheduleInit.runOnce(world);
		}

		void update();

		void TearDown() override
		{
			ZT_TIME_LOG(deinit());
		}

		void deinit()
		{
			scheduleDeinit.addSystem(Window{}, Window::Deinit, ecs::MainThread{});
			scheduleDeinit.addSystem(Renderer{}, Renderer::Deinit, ecs::Before{ Window{} }, ecs::MainThread{});
			scheduleDeinit.addSystem(ImGui{}, ImGui::Deinit, ecs::Before{ Window{}, Renderer{} }, ecs::MainThread{});
			scheduleDeinit.addSystem(Sprites{}, Sprites::Deinit);

			scheduleDeinit.buildGraph();
			scheduleDeinit.resolveGraph();
			scheduleDeinit.runOnce(world);
		}

		ecs::World world;

		ecs::Schedule scheduleInit;
		ecs::Schedule scheduleUpdate;
		ecs::Schedule scheduleDeinit; 
	};

	TEST_F(EngineTests, Test)
	{
		ZT_TIME_LOG(update());
	}

	void EngineTests::update()
	{
		scheduleUpdate.addSystem(Window{}, Window::Update, ecs::MainThread{});
		scheduleUpdate.addSystem(Renderer{}, Renderer::Update, ecs::After{ Window{} }, ecs::MainThread{});
		scheduleUpdate.addSystem(ImGui::Pre{}, ImGui::PreUpdate, ecs::Before{ Renderer{} }, ecs::After{ Window{} }, ecs::MainThread{});
		scheduleUpdate.addSystem(ImGui::Post{}, ImGui::PostUpdate, ecs::Before{ Renderer{} }, ecs::After{ ImGui::Pre{} }, ecs::MainThread{});

		scheduleUpdate.addSystem(Editor{}, Editor::EntryPoint, ecs::After{ ImGui::Pre{} }, ecs::Before{ ImGui::Post{} }, ecs::MainThread{});

		scheduleUpdate.addSystem(Sprites{}, Sprites::Update, ecs::Before{ Renderer{}, Editor{} });

		scheduleUpdate.buildGraph();
		scheduleUpdate.resolveGraph();

		// Add a ptr to the graph as a resource so we can access it from systems
		const auto* graph = &scheduleUpdate.getGraph();
		world.addResource(graph);

		while (true)
		{
			auto exitReason = world.getResource<ExitReason>();
			if (exitReason)
			{
				switch (exitReason->level)
				{
				case ExitReason::Info:
					Logger->info("Exit because: {}", exitReason->reason);
					break;
				case ExitReason::Error:
					Logger->error("Exit because: {}", exitReason->reason);
					break;
				case ExitReason::Critical:
					Logger->critical("Exit because: {}", exitReason->reason);
					break;
				}
				break;
			}

			scheduleUpdate.runOnce(world);

			if (!IsDebuggerAttached())
				break;
		}
	}
}