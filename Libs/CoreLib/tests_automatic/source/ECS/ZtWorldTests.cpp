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

	TEST_F(ECSWorldTests, IDTest)
	{
		Entity entity_0 = world.spawn();
		ASSERT_EQ(entity_0.id(), 0);

		Entity entity_1 = world.spawn();
		ASSERT_EQ(entity_1.id(), 1);
	}

	TEST_F(ECSWorldTests, GetComponentsTest)
	{
		world.spawn(Position{});
		world.spawn(Position{}, Velocity{});

		[[maybe_unused]] auto positions = world.getComponents<Position>();
	}
}

/*
World
{
	Chunk<Position>
	Chunk<Position, Velocity>
	Chunk<Position, Sprite>
	...
	...
	...
}

Chunk<Position, Velocity, ..., N>
{
	std::vector<Position> components_0;
	std::vector<Velocity> components_1;
	...
	...
	...
	std::vector<N> components_Index_N;
}

Chunk<Position, Velocity, ..., N>
{
	std::vector<Position> components_0;
	std::vector<Velocity> components_1;
	...
	...
	...
	std::vector<N> components_Index_N;
}

*/