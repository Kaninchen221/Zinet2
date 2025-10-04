#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSWorldTests : public ::testing::Test
	{
	protected:

		World world;
	};

	TEST_F(ECSWorldTests, SpawnEntityIDTest)
	{
		Entity entity_0 = world.spawn();
		ASSERT_EQ(entity_0.getID(), 0);

		Entity entity_1 = world.spawn();
		ASSERT_EQ(entity_1.getID(), 1);
	}

	TEST_F(ECSWorldTests, SpawnOneEntityWithOneComponentTest)
	{
		const Sprite expected{ 56 };
		Entity entity = world.spawn(expected);

		ASSERT_EQ(world.getArchetypesCount(), 1);

		const auto actual = world.getComponent<Sprite>(entity);
		ASSERT_TRUE(actual);
		ASSERT_EQ(*actual, expected);
	}

	TEST_F(ECSWorldTests, SpawnOneEntityWithMultipleComponentsTest)
	{
		const Sprite expectedSprite{ 56 };
		const Position expectedPosition{ 2, 4 };
		const Velocity expectedVelocity{ 10, 20 };
		Entity entity = world.spawn(expectedSprite, expectedPosition, expectedVelocity);

		ASSERT_EQ(world.getArchetypesCount(), 1);

		const auto actualSprite = world.getComponent<Sprite>(entity);
		const auto actualPosition = world.getComponent<Position>(entity);
		const auto actualVelocity = world.getComponent<Velocity>(entity);
		
		ASSERT_TRUE(actualPosition);
		ASSERT_EQ(*actualPosition, expectedPosition);

		ASSERT_TRUE(actualSprite);
		ASSERT_EQ(*actualSprite, expectedSprite);

		ASSERT_TRUE(actualVelocity);
		ASSERT_EQ(*actualVelocity, expectedVelocity);
	}

	TEST_F(ECSWorldTests, SpawnMultipleEntitiesWithMultipleComponentsTest)
	{
		std::vector<Entity> entities;
		const auto count = 5;
		for (size_t i = 0; i < count; i++)
		{
			const auto value = static_cast<float>(i);
			auto entity = world.spawn(Position{ value, value }, Velocity{ value, value }, Sprite{ static_cast<int>(i) });
			entities.push_back(entity);
		}

		for (size_t i = 0; i < count; i++)
		{
			auto& entity = entities[i];

			const auto actualSprite = world.getComponent<Sprite>(entity);
			const auto actualPosition = world.getComponent<Position>(entity);
			const auto actualVelocity = world.getComponent<Velocity>(entity);

			const auto value = static_cast<float>(i);

			ASSERT_TRUE(actualPosition);
			const auto expectedPosition = Position{ value, value };
			ASSERT_EQ(*actualPosition, expectedPosition);

			ASSERT_TRUE(actualVelocity);
			const auto expectedVelocity = Velocity{ value, value };
			ASSERT_EQ(*actualVelocity, expectedVelocity);

			ASSERT_TRUE(actualSprite);
			ASSERT_EQ(*actualSprite, Sprite{ static_cast<int>(i) });
		}
	}

	TEST_F(ECSWorldTests, MultipleArchetypesTest)
	{
		const auto entityStaticSprite = world.spawn(Position{}, Sprite{});
		const auto entitySprite = world.spawn(Position{}, Velocity{}, Sprite{});

		ASSERT_TRUE(world.getComponent<Position>(entityStaticSprite));
		ASSERT_TRUE(world.getComponent<Sprite>(entityStaticSprite));
		ASSERT_FALSE(world.getComponent<Velocity>(entityStaticSprite));

		ASSERT_TRUE(world.getComponent<Position>(entitySprite));
		ASSERT_TRUE(world.getComponent<Sprite>(entitySprite));
		ASSERT_TRUE(world.getComponent<Velocity>(entitySprite));
	}
}