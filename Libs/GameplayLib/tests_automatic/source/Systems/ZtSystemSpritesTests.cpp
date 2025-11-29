#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

namespace zt::gameplay::system::tests
{
	using namespace zt::core;
	using namespace zt::vulkan_renderer;

	class SpritesTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(SpritesTests, Test)
	{
		ecs::World world;
		ecs::Schedule schedule;

		// TODO: Write something like Schedule::runSystemOnce(SystemLabel, System, World&)? 
		// So we could skip writing addSystem, buildGraph etc.

		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(200, 200));

		auto rendererRes = world.addResource(VulkanRenderer{});
		ASSERT_TRUE(rendererRes->init(window));

		AssetsStorage assetsStorage;
		//assetsStorage.registerAssetClass<asset::Texture>();
		assetsStorage.registerAssetClass<asset::Shader>();
		assetsStorage.registerAssetClass<asset::Sampler>();
		assetsStorage.storeAssets();

		world.addResource(assetsStorage);

		{ // Init
			schedule.addSystem(Sprites{}, Sprites::Init);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();

			// TODO: Now work on descriptors

// 			ecs::ConstQuery<GraphicsPipeline> query{ world };
// 			EXPECT_EQ(query.getComponentsCount(), 1);
			
// 			for (auto [graphicsPipeline] : query)
// 			{
// 				EXPECT_TRUE(graphicsPipeline->isValid());
// 			}

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		{ // Update
			schedule.addSystem(Sprites{}, Sprites::Update);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		{ // Deinit
			schedule.addSystem(Sprites{}, Sprites::Deinit);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		rendererRes->deinit();
		window.destroyWindow();
		wd::GLFW::Deinit();
	}
}