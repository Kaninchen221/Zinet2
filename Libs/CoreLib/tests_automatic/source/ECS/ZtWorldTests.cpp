#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{
	class ECSWorldTests : public ::testing::Test
	{
	protected:

		World world;

		struct Position { float x; float y; };
		struct Velocity { float x; float y; };
		struct Sprite { int textureID; };
	};

	TEST_F(ECSWorldTests, SpawnEntityIDTest)
	{
		Entity entity_0 = world.spawn();
		ASSERT_EQ(entity_0.id(), 0);

		Entity entity_1 = world.spawn();
		ASSERT_EQ(entity_1.id(), 1);
	}

	TEST_F(ECSWorldTests, SpawnOneEntityWithOneComponentTest)
	{
		const Position expectedPosition{ 1, 2 };
		Entity entity = world.spawn(expectedPosition);

		//const auto component = world.getComponent<Position>(entity);
		//ASSERT_TRUE(component);
	}
}