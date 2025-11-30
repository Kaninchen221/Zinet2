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

	TEST_F(ECSWorldCommandsTests, SpawnTrivialTypeTest)
	{
		const int expectedValue = 40;

		{
			WorldCommands worldCommands{ world };
			worldCommands.spawn(int{ expectedValue });
		}
		world.executeCommands();
		world.clearCommands();

		ASSERT_EQ(world.getComponentsCount(), 1);
		Query<int> query{ world };
		for (auto [component] : query)
		{
			EXPECT_EQ(*component, expectedValue);
		}
	}

	TEST_F(ECSWorldCommandsTests, SpawnEntityNonMovableClassTest)
	{
		// TODO: Handle non copyable classes
		/*
		const int expectedValue = 40;

		{
			WorldCommands worldCommands{ world };
			worldCommands.spawn(NonMovableClass{ expectedValue });
		}
		world.executeCommands();
		world.clearCommands();

		ASSERT_EQ(world.getComponentsCount(), 1);
		Query<NonMovableClass> query{ world };
		for (auto [component] : query)
		{
			EXPECT_EQ(component->value, expectedValue);
		}
		*/
	}

	TEST_F(ECSWorldCommandsTests, SpawnEntityNonCopyableClassTest)
	{
		const int expectedValue = 40;

		{
			WorldCommands worldCommands{ world };
			worldCommands.spawn(NonCopyableClass{ expectedValue });
		}
		world.executeCommands();
		world.clearCommands();

		ASSERT_EQ(world.getComponentsCount(), 1);
		Query<NonCopyableClass> query{ world };
		for (auto [component] : query)
		{
			EXPECT_EQ(component->value, expectedValue);
		}
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

	// TODO: Write a test that will test AddResource for an object of a class that is non copyable
}