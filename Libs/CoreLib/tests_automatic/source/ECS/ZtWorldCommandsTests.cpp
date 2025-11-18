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
	};

	TEST_F(ECSWorldCommandsTests, SpawnEntityTest)
	{
		{
			WorldCommands worldCommands{ world };
			worldCommands.spawn(Position{}, Velocity{});
		}

		world.executeCommands();

		ASSERT_EQ(world.getComponentsCount(), 2);
	}

	TEST_F(ECSWorldCommandsTests, RemoveEntityTest)
	{
		Entity entity{ {}, {} };
		{
			WorldCommands worldCommands{ world };
			entity = world.spawn(Position{}, Velocity{});
		}
		ASSERT_EQ(world.getComponentsCount(), 2);

		{
			WorldCommands worldCommands{ world };
			worldCommands.remove(entity);
		}

		world.executeCommands();

		ASSERT_EQ(world.getComponentsCount(), 0);
	}

	TEST_F(ECSWorldCommandsTests, AddResourceTest)
	{
		{
			WorldCommands worldCommands{ world };
			worldCommands.addResource(Position{});
		}

		world.executeCommands();

		ASSERT_TRUE(world.getResource<Position>());
	}
}