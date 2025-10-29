#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtTimeLog.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

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
			: scheduleInit{ ecs::Schedule::Create(MainThread, GameplayThread) },
			scheduleUpdate{ ecs::Schedule::Create(MainThread, GameplayThread) },
			scheduleDeinit{ ecs::Schedule::Create(MainThread, GameplayThread) }
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
			AssetsStorage assetsStorage;
			assetsStorage.registerAssetClass<core::asset::Text>();
			assetsStorage.registerAssetClass<asset::Texture>();
			assetsStorage.registerAssetClass<asset::Shader>();
			assetsStorage.registerAssetClass<asset::Sampler>();
			ZT_TIME_LOG(assetsStorage.storeAssets());

			world.addResource(assetsStorage);

			world.addResource(components::ExitReason{});
		}

		void init()
		{
			scheduleInit.addSystem(system::Window{}, system::Window::Init, MainThread);
			scheduleInit.addSystem(system::Renderer{}, system::Renderer::Init, MainThread);
			scheduleInit.addSystem(system::ImGui{}, system::ImGui::Init, MainThread);

			scheduleInit.requestStop();
			scheduleInit.run(world, MainThread);
			scheduleInit.waitForStop();
		}

		void update();

		void TearDown() override
		{
			ZT_TIME_LOG(deinit());
		}

		void deinit()
		{
			scheduleDeinit.addSystem(system::ImGui{}, system::ImGui::Deinit, MainThread);
			scheduleDeinit.addSystem(system::Renderer{}, system::Renderer::Deinit, MainThread);
			scheduleDeinit.addSystem(system::Window{}, system::Window::Deinit, MainThread);

			scheduleDeinit.requestStop();
			scheduleDeinit.run(world, MainThread);
			scheduleDeinit.waitForStop();
		}

		enum Threads : ecs::ThreadID
		{
			MainThread, // Window, Renderer, ImGui
			GameplayThread, // Game Logic
		};;

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
		scheduleUpdate.addSystem(system::ImGui{}, system::ImGui::Update, MainThread);
		scheduleUpdate.addSystem(system::Renderer{}, system::Renderer::Update, MainThread); // System Renderer must be before System Window
		scheduleUpdate.addSystem(system::Window{}, system::Window::Update, MainThread);

		auto exitReason = world.getResource<components::ExitReason>();
		if (!exitReason)
		{
			Logger->error("Couldn't find an exit reason resource");
			return;
		}

		while (!exitReason->exit)
		{
			if (!IsDebuggerAttached())
				break;

			scheduleUpdate.run(world, MainThread);
		}

		Logger->info("ExitReason: {}", exitReason->reason);

		scheduleUpdate.requestStop();
		scheduleUpdate.waitForStop();
	}
}