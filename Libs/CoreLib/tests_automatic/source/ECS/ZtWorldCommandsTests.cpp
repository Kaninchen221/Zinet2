#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	using namespace zt::core::tests;

	class ECSWorldCommandsTests : public ::testing::Test
	{
	protected:
		World world;

		template<class ComponentT>
		void testSpawnComponent();

		template<class ResourceT>
		void testAddResource();
	};

	TEST_F(ECSWorldCommandsTests, SpawnEntityTrivialTypeTest)
	{
		testSpawnComponent<int>();
	}

	TEST_F(ECSWorldCommandsTests, SpawnEntityNonTrivialTypeTest)
	{
		testSpawnComponent<NonTrivialClass>();
	}

	TEST_F(ECSWorldCommandsTests, SpawnEntityNonCopyableTypeTest)
	{
		testSpawnComponent<NonCopyableClass>();
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

	TEST_F(ECSWorldCommandsTests, AddResourceTrivialTypeTest)
	{
		testAddResource<TrivialClass>();
	}

	TEST_F(ECSWorldCommandsTests, AddResourceNonCopyableTypeTest)
	{
		testAddResource<NonCopyableClass>();
	}

	template<class ComponentT>
	void ECSWorldCommandsTests::testSpawnComponent()
	{
		const int expectedValue = 40;

		{
			WorldCommands worldCommands{ world };
			world.spawn(ComponentT{ expectedValue });
		}

		world.executeCommands();

		ASSERT_EQ(world.getComponentsCount(), 1);
		Query<ComponentT> query{ world };
		for (auto [component] : query)
		{
			ASSERT_TRUE(component);

			if constexpr (requires { ComponentT::value; })
				ASSERT_EQ(component->value, expectedValue);
			else
				ASSERT_EQ(*component, expectedValue);
		}
	}

	template<class ResourceT>
	void ECSWorldCommandsTests::testAddResource()
	{
		const int expectedValue = 50;

		{
			WorldCommands worldCommands{ world };
			worldCommands.addResource(NonCopyableClass{ expectedValue });
		}

		world.executeCommands();

		auto resource = world.getResource<NonCopyableClass>();
		ASSERT_TRUE(resource);
		ASSERT_EQ(resource->value, expectedValue);
	}
}