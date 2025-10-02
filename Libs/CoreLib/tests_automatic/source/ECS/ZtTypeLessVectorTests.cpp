#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

namespace zt::core::ecs
{
	class ECSTypeLessVectorTests : public ::testing::Test
	{
	protected:

		struct Position { float x; float y; };
		struct Sprite { int textureID; };
	};

	TEST_F(ECSTypeLessVectorTests, CreateTest)
	{
		const Position expectedPosition{ 1.0f, 2.0f };
		TypeLessVector components = TypeLessVector::Create<Position>();
		
		ASSERT_EQ(components.getTypeInfo(), typeid(Position));
		ASSERT_EQ(components.getSize(), 0);
	}

	TEST_F(ECSTypeLessVectorTests, GetFromInvalidIndexTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		auto sprite = components.get<Sprite>(0);

		ASSERT_FALSE(sprite);
	}

	TEST_F(ECSTypeLessVectorTests, AddTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		const Sprite expectedFirstSprite{ 2 };
		ASSERT_EQ(0, components.add(expectedFirstSprite));

		const Sprite expectedSecondSprite{ 1 };
		ASSERT_EQ(1, components.add(expectedSecondSprite));

		auto actualFirstSprite = components.get<Sprite>(0);
		ASSERT_TRUE(actualFirstSprite);
		ASSERT_EQ(actualFirstSprite->textureID, expectedFirstSprite.textureID);

		auto actualSecondSprite = components.get<Sprite>(1);
		ASSERT_TRUE(actualSecondSprite);
		ASSERT_EQ(actualSecondSprite->textureID, expectedSecondSprite.textureID);
	}

	TEST_F(ECSTypeLessVectorTests, SizeTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		components.add(Position{});
		components.add(Position{});
		components.add(Position{});

		ASSERT_EQ(components.getSize(), 3);
	}

	TEST_F(ECSTypeLessVectorTests, RemoveTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		ASSERT_EQ(0, components.add(Sprite{0}));
		components.remove(0);

		ASSERT_EQ(0, components.add(Sprite{0}));
		ASSERT_EQ(1, components.add(Sprite{1}));
		ASSERT_EQ(2, components.add(Sprite{2}));

		{
			components.remove(1);
			ASSERT_EQ(components.getSize(), 2);

			auto component = components.get<Sprite>(1);
			ASSERT_FALSE(component);

			// Removing component at index that contains removed component shouldn't change the size
			components.remove(1);
			ASSERT_EQ(components.getSize(), 2);
		}
	}

	TEST_F(ECSTypeLessVectorTests, HasTypeTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		ASSERT_TRUE(components.hasType<Sprite>());
		ASSERT_FALSE(components.hasType<Position>());

	}
}