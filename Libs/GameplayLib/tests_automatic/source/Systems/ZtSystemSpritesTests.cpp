#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

namespace zt::gameplay::system::tests
{
	using namespace zt::core;

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

		{ // Init
			schedule.addSystem(Sprites{}, Sprites::Init);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();
		}

		{ // Update
			schedule.addSystem(Sprites{}, Sprites::Update);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();
		}

		{ // Deinit
			schedule.addSystem(Sprites{}, Sprites::Deinit);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();
		}
	}
}