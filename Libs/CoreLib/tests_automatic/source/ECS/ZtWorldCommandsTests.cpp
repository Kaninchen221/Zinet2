#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSWorldCommandsTests : public ::testing::Test
	{
	protected:
		World world;
		WorldCommands worldCommands{ world };
	};

	TEST_F(ECSWorldCommandsTests, SpawnEntityTest)
	{
		worldCommands.spawn(Position{}, Velocity{});

		worldCommands.executeCommands(world);

		ASSERT_EQ(world.getComponentsCount(), 2);
	}

	TEST_F(ECSWorldCommandsTests, RemoveEntityTest)
	{
		auto entity = world.spawn(Position{}, Velocity{});
		ASSERT_EQ(world.getComponentsCount(), 2);

		worldCommands.remove(entity);

		worldCommands.executeCommands(world);

		ASSERT_EQ(world.getComponentsCount(), 0);
	}

	TEST_F(ECSWorldCommandsTests, AddResourceTest)
	{
		worldCommands.addResource(Position{});

		worldCommands.executeCommands(world);

		ASSERT_TRUE(world.getResource<Position>());
	}
}