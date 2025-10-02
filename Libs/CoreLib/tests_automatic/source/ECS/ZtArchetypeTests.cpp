#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtArchetype.hpp"

namespace zt::core::ecs
{
	class ECSArchetypeTests : public ::testing::Test
	{
	protected:

		struct Position { float x; float y; };
		struct Sprite { int id; };
		struct Velocity { float x; float y; };
	};

	TEST_F(ECSArchetypeTests, CreateTest)
	{
		Archetype archetype = Archetype::Create<Position, Sprite>();
	}

	TEST_F(ECSArchetypeTests, GetComponentsTest)
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

		{ // Invalid because the arcvhetype doesn't have velocity
			const bool result = archetype.hasTypes<Position, Velocity>();
			ASSERT_FALSE(result);
		}
	}
}