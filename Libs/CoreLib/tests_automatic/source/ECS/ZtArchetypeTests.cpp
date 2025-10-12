#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtArchetype.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSArchetypeTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(ECSArchetypeTests, CreateTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();
	}

	TEST_F(ECSArchetypeTests, AddTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();

		const Entity firstEntity{ 0, 0 };
		const Position expectedPosition{ 1, 2 };
		const Sprite expectedSprite{ 10 };
		const auto index = archetype.add(firstEntity, Position(expectedPosition), Sprite(expectedSprite));
		ASSERT_EQ(index, 0);

		ASSERT_TRUE(archetype.hasEntity(firstEntity));

		auto* positionComponents = archetype.getComponentsOfType<Position>();
		ASSERT_EQ(positionComponents->getSize(), 1);
		
		const auto actualPosition = positionComponents->get<Position>(index);
		ASSERT_TRUE(actualPosition);
		ASSERT_EQ(*actualPosition, expectedPosition);

		auto* spriteComponents = archetype.getComponentsOfType<Sprite>();
		ASSERT_EQ(spriteComponents->getSize(), 1);

		const auto actualSprite = spriteComponents->get<Sprite>(index);
		ASSERT_TRUE(actualSprite);
		ASSERT_EQ(*actualSprite, expectedSprite);

		const Entity secondEntity{ 1, 0 };
		ASSERT_EQ(archetype.add(secondEntity, Position{}, Sprite{}), 1);

		ASSERT_TRUE(archetype.hasEntity(secondEntity));
	}

	TEST_F(ECSArchetypeTests, TryAddInvalidCountOfComponentsTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();

		const auto index = archetype.add(Entity{0, 0}, Position{});
		ASSERT_EQ(index, InvalidIndex);
	}

	TEST_F(ECSArchetypeTests, RemoveComponentsTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();
		Entity entity{ 0 /*ID*/, InvalidIndex /*ComponentsIndex*/ };
		auto componentsIndex = archetype.add(entity, Position{ 1, 2 }, Sprite{ 50 });
		entity = Entity{ 0 /*ID*/, componentsIndex /*ComponentsIndex*/ };

		ASSERT_TRUE(archetype.hasEntity(entity));

		ASSERT_TRUE(archetype.remove(entity));
		ASSERT_FALSE(archetype.hasEntity(entity));

		auto positions = archetype.getComponentsOfType<Position>();
		ASSERT_TRUE(positions);
		auto componentPosition = positions->get<Position>(entity.getComponentsIndex());
		ASSERT_FALSE(componentPosition);

		auto sprites = archetype.getComponentsOfType<Sprite>();
		ASSERT_TRUE(sprites);
		auto componentSprite = sprites->get<Sprite>(entity.getComponentsIndex());
		ASSERT_FALSE(componentSprite);
	}

	TEST_F(ECSArchetypeTests, GetComponentOfTypeTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();
		const Position expectedPosition{ 34, 2 };
		const Sprite expectedSprite{ 40 };
		archetype.add(Entity{ 0, 0 }, Position(expectedPosition), Sprite(expectedSprite));
		archetype.add(Entity{ 0, 0 }, Position{ 1, 1 }, Sprite{ 10 });

		auto actualPosition = archetype.getComponentOfType<Position>(0);
		ASSERT_TRUE(actualPosition);
		ASSERT_EQ(*actualPosition, expectedPosition);

		auto actualSprite = archetype.getComponentOfType<Sprite>(0);
		ASSERT_TRUE(actualSprite);
		ASSERT_EQ(*actualSprite, expectedSprite);
	}

	TEST_F(ECSArchetypeTests, GetComponentsOfTypeTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();

		auto* positionComponents = archetype.getComponentsOfType<Position>();
		ASSERT_TRUE(positionComponents);

		auto* spriteComponents = archetype.getComponentsOfType<Sprite>();
		ASSERT_TRUE(spriteComponents);
	}

	TEST_F(ECSArchetypeTests, GetComponentsWithInvalidTypeTest)
	{
		Archetype archetype = Archetype::Create<Position>();

		auto* invalidComponents = archetype.getComponentsOfType<Velocity>();
		ASSERT_FALSE(invalidComponents);
	}

	// I'm getting strange compilation error in ASSERT_TRUE macro when testing more than one type 
	// So I put the result in a variable
	TEST_F(ECSArchetypeTests, HasTypesTest)
	{
		const Archetype archetype = Archetype::Create<Position, Sprite>();

		{ // Typical test
			const bool result = archetype.hasTypes<Position, Sprite>();
			ASSERT_TRUE(result);
		}

		{ // Reverse order
			const bool result = archetype.hasTypes<Sprite, Position>();
			ASSERT_TRUE(result);
		}

		{ // Type occurs more than one time
			const bool result = archetype.hasTypes<Sprite, Position, Sprite>();
			ASSERT_TRUE(result);
		}

		// The archetype doesn't have the Velocity type
		ASSERT_FALSE(archetype.hasTypes<Velocity>());

		{ // Invalid because the archetype doesn't have velocity
			const bool result = archetype.hasTypes<Position, Velocity>();
			ASSERT_FALSE(result);
		}
	}

	TEST_F(ECSArchetypeTests, TypesEqualTest)
	{
		const Archetype archetype = Archetype::Create<Position, Sprite>();

		{ // Types order like in the Create function
			const bool result = archetype.typesEqual<Position, Sprite>();
			ASSERT_TRUE(result);
		}

		{ // Types in reverse order
			const bool result = archetype.typesEqual<Sprite, Position>();
			ASSERT_TRUE(result);
		}

		// Invalid because types count is different
		ASSERT_FALSE(archetype.typesEqual<Position>());

		// Invalid because archetype doesn't have the Velocity type
		{
			const bool result = archetype.typesEqual<Position, Sprite, Velocity>();
			ASSERT_FALSE(result);
		}
	}

	TEST_F(ECSArchetypeTests, GetEntitiesCountTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();

		const Entity entity{ 0, 0 };
		archetype.add(entity, Position{}, Sprite{});
		ASSERT_EQ(archetype.getEntitiesCount(), 1);

		const Entity entity2{ 1, 0 };
		archetype.add(entity2, Position{}, Sprite{});
		ASSERT_EQ(archetype.getEntitiesCount(), 2);
	}

	TEST_F(ECSArchetypeTests, GetComponentsCountTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();

		const Entity entity{ 0, 0 };
		archetype.add(entity, Position{}, Sprite{});
		ASSERT_EQ(archetype.getComponentsCount(), 2);

		archetype.add(entity, Position{}, Sprite{});
		archetype.add(entity, Position{}, Sprite{});
		ASSERT_EQ(archetype.getComponentsCount(), 6);
	}
}