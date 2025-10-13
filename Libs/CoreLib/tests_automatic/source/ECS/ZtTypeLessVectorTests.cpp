#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include <ranges>
#include <numeric>

namespace zt::core::ecs::tests
{
	class ECSTypeLessVectorTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(ECSTypeLessVectorTests, CreateTest)
	{
		const Position expectedPosition{ 1.0f, 2.0f };
		TypeLessVector components = TypeLessVector::Create<Position>();
		
		ASSERT_EQ(components.getTypeID(), GetTypeID<Position>());
		ASSERT_EQ(components.getComponentsCount(), 0);
	}

	TEST_F(ECSTypeLessVectorTests, GetFromInvalidIndexTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		auto sprite = components.get<Sprite>(0);

		ASSERT_FALSE(sprite);
	}

	TEST_F(ECSTypeLessVectorTests, IsValidIndexTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		components.add(Position{});
		components.add(Position{});
		components.add(Position{});

		const size_t removedComponentIndex = 1;
		ASSERT_TRUE(components.remove(removedComponentIndex));

		ASSERT_TRUE(components.isValidIndex(0));
		ASSERT_TRUE(components.isValidIndex(2));
		ASSERT_FALSE(components.isValidIndex(removedComponentIndex));
		ASSERT_FALSE(components.isValidIndex(3)); // Out of bounds index
	}

	TEST_F(ECSTypeLessVectorTests, GetFirstValidIndexTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();

		ASSERT_EQ(components.getFirstValidIndex(), InvalidIndex);

		components.add(Position{}); // 0
		components.add(Position{});	// 1
		components.add(Position{});	// 2
		components.add(Position{});	// 3

		components.remove(0);
		components.remove(1);

		const size_t expectedFirstValidIndex = 2;
		const size_t actualFirstValidIndex = components.getFirstValidIndex();
		ASSERT_EQ(actualFirstValidIndex, expectedFirstValidIndex);

		components.remove(2);
		components.remove(3);
		ASSERT_EQ(components.getFirstValidIndex(), InvalidIndex);
	}

	TEST_F(ECSTypeLessVectorTests, AddTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		const Sprite expectedFirstSprite{ 2 };
		ASSERT_EQ(0, components.add(Sprite(expectedFirstSprite)));

		const Sprite expectedSecondSprite{ 1 };
		ASSERT_EQ(1, components.add(Sprite(expectedSecondSprite)));

		auto actualFirstSprite = components.get<Sprite>(0);
		ASSERT_TRUE(actualFirstSprite);
		ASSERT_EQ(actualFirstSprite->id, expectedFirstSprite.id);

		auto actualSecondSprite = components.get<Sprite>(1);
		ASSERT_TRUE(actualSecondSprite);
		ASSERT_EQ(actualSecondSprite->id, expectedSecondSprite.id);
	}

	TEST_F(ECSTypeLessVectorTests, GetComponentsCountTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		components.add(Position{});
		components.add(Position{});
		components.add(Position{});

		EXPECT_TRUE(components.remove(1));

		ASSERT_EQ(components.getComponentsCount(), 2);
	}

	TEST_F(ECSTypeLessVectorTests, GetComponentsCapacityTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		components.add(Position{});
		components.add(Position{});
		components.add(Position{});

		EXPECT_TRUE(components.remove(1));

		ASSERT_EQ(components.getComponentsCapacity(), 3);
	}

	TEST_F(ECSTypeLessVectorTests, RemoveTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		ASSERT_EQ(0, components.add(Sprite{0}));
		ASSERT_TRUE(components.remove(0));

		ASSERT_EQ(0, components.add(Sprite{0}));
		ASSERT_EQ(1, components.add(Sprite{1}));
		ASSERT_EQ(2, components.add(Sprite{2}));

		{
			ASSERT_TRUE(components.remove(1));
			ASSERT_EQ(components.getComponentsCount(), 2);

			auto component = components.get<Sprite>(1);
			ASSERT_FALSE(component);

			// Removing component at index that contains removed component shouldn't change the size
			ASSERT_FALSE(components.remove(1));
			ASSERT_EQ(components.getComponentsCount(), 2);
		}
	}

	TEST_F(ECSTypeLessVectorTests, HasTypeTest)
	{
		TypeLessVector components = TypeLessVector::Create<Sprite>();

		ASSERT_TRUE(components.hasType<Sprite>());
		ASSERT_FALSE(components.hasType<Position>());
	}

	TEST_F(ECSTypeLessVectorTests, AddTypeWithComplexDataLikeSTDVectorTest)
	{
		using Type = NotTrivialType;
		TypeLessVector components = TypeLessVector::Create<Type>();

		std::string expectedName = "TestName";
		std::vector<int> expectedData(100);
		std::iota(expectedData.begin(), expectedData.end(), 0);
		std::string expectedDescription = "TestDescription";

		size_t componentIndex = InvalidIndex;
		constexpr size_t count = 10;
		for (size_t i = 0; i < count; ++i)
		{
			Type complex;
			complex.name = expectedName;
			complex.data = expectedData;
			complex.description = expectedDescription;

			componentIndex = components.add(complex);
			ASSERT_NE(componentIndex, InvalidIndex);
		}

		const size_t removedComponentIndex = 4;
		components.remove(removedComponentIndex);

		for (size_t i = 0; i < count; ++i)
		{
			if (i == removedComponentIndex)
				continue;

			auto resource = components.get<Type>(i);
			ASSERT_TRUE(resource);

			ASSERT_EQ(expectedName, resource->name);

			ASSERT_EQ(expectedData.size(), resource->data.size());
			for (const auto& [expected, actual] : std::views::zip(expectedData, resource->data))
			{
				EXPECT_EQ(expected, actual);
			}

			ASSERT_EQ(expectedDescription, resource->description);
		}
	}
}
