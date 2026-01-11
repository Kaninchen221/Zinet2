#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Zinet/Core/ZtTypeLessVector.hpp"
#include "Zinet/Core/ZtTypes.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include <ranges>
#include <numeric>

namespace zt::core::tests
{
	using namespace zt::core::ecs;

	class TypeLessVectorTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(TypeLessVectorTests, CreateTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		
		ASSERT_EQ(vector.getTypeID(), GetTypeID<Position>());
		ASSERT_EQ(vector.getObjectsCount(), 0);
	}

	TEST_F(TypeLessVectorTests, ClearTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{});
		vector.add(Position{});
		vector.add(Position{});

		ASSERT_EQ(vector.getObjectsCount(), 3);

		vector.clear();
		EXPECT_EQ(vector.getObjectsCount(), 0);
		EXPECT_EQ(vector.getObjectsCapacity(), 0);
		EXPECT_TRUE(vector.isEmpty());
	}

	TEST_F(TypeLessVectorTests, GetFromInvalidIndexTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		auto sprite = vector.get<Sprite>(0);

		ASSERT_FALSE(sprite);
	}

	TEST_F(TypeLessVectorTests, GetPtrTest)
	{
		const Position expectedPosition{ 1, 1 };

		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{ 0, 0 });
		vector.add(Position{ expectedPosition });
		vector.add(Position{ 2, 2 });

		ASSERT_EQ(vector.getObjectsCount(), 3);

		EXPECT_EQ(vector.data(), vector.getPtr(0));

		const auto secondObject = static_cast<Position*>(vector.getPtr(1));
		EXPECT_EQ(expectedPosition, *secondObject);
	}

	TEST_F(TypeLessVectorTests, GetLastIndexTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{});
		vector.add(Position{});
		vector.add(Position{});
		vector.remove(2);

		ASSERT_EQ(vector.getObjectsCount(), 2);

		size_t expectedLastIndex = 2;
		size_t actualLastIndex = vector.getLastIndex();
		EXPECT_EQ(actualLastIndex, expectedLastIndex);
	}

	TEST_F(TypeLessVectorTests, IsValidIndexTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{});
		vector.add(Position{});
		vector.add(Position{});

		const size_t removedComponentIndex = 1;
		ASSERT_TRUE(vector.remove(removedComponentIndex));

		ASSERT_TRUE(vector.isValidIndex(0));
		ASSERT_TRUE(vector.isValidIndex(2));
		ASSERT_FALSE(vector.isValidIndex(removedComponentIndex));
		ASSERT_FALSE(vector.isValidIndex(3)); // Out of bounds index
	}

	TEST_F(TypeLessVectorTests, GetFirstValidIndexTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();

		ASSERT_EQ(vector.getFirstValidIndex(), InvalidIndex);

		vector.add(Position{}); // 0
		vector.add(Position{});	// 1
		vector.add(Position{});	// 2
		vector.add(Position{});	// 3

		vector.remove(0);
		vector.remove(1);

		const size_t expectedFirstValidIndex = 2;
		const size_t actualFirstValidIndex = vector.getFirstValidIndex();
		ASSERT_EQ(actualFirstValidIndex, expectedFirstValidIndex);

		vector.remove(2);
		vector.remove(3);
		ASSERT_EQ(vector.getFirstValidIndex(), InvalidIndex);
	}

	TEST_F(TypeLessVectorTests, AddTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Sprite>();

		const Sprite expectedFirstSprite{ 2 };
		ASSERT_EQ(0, vector.add(Sprite(expectedFirstSprite)));

		const Sprite expectedSecondSprite{ 1 };
		ASSERT_EQ(1, vector.add(Sprite(expectedSecondSprite)));

		auto actualFirstSprite = vector.get<Sprite>(0);
		ASSERT_TRUE(actualFirstSprite);
		ASSERT_EQ(actualFirstSprite->id, expectedFirstSprite.id);

		auto actualSecondSprite = vector.get<Sprite>(1);
		ASSERT_TRUE(actualSecondSprite);
		ASSERT_EQ(actualSecondSprite->id, expectedSecondSprite.id);
	}

	TEST_F(TypeLessVectorTests, GetVectorCountTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{});
		vector.add(Position{});
		vector.add(Position{});

		EXPECT_TRUE(vector.remove(1));

		ASSERT_EQ(vector.getObjectsCount(), 2);
	}

	TEST_F(TypeLessVectorTests, GetVectorCapacityTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Position>();
		vector.add(Position{});
		vector.add(Position{});
		vector.add(Position{});

		EXPECT_TRUE(vector.remove(1));

		ASSERT_EQ(vector.getObjectsCapacity(), 3);
	}

	TEST_F(TypeLessVectorTests, RemoveTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Sprite>();

		ASSERT_EQ(0, vector.add(Sprite{0}));
		ASSERT_TRUE(vector.remove(0));

		ASSERT_EQ(0, vector.add(Sprite{0}));
		ASSERT_EQ(1, vector.add(Sprite{1}));
		ASSERT_EQ(2, vector.add(Sprite{2}));

		{
			ASSERT_TRUE(vector.remove(1));
			ASSERT_EQ(vector.getObjectsCount(), 2);

			auto component = vector.get<Sprite>(1);
			ASSERT_FALSE(component);

			// Removing component at index that contains removed component shouldn't change the size
			ASSERT_FALSE(vector.remove(1));
			ASSERT_EQ(vector.getObjectsCount(), 2);
		}
	}

	TEST_F(TypeLessVectorTests, HasTypeTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Sprite>();

		ASSERT_TRUE(vector.hasType<Sprite>());
		ASSERT_FALSE(vector.hasType<Position>());
	}

	TEST_F(TypeLessVectorTests, AddTypeWithComplexDataLikeSTDVectorTest)
	{
		ASSERT_EQ(NotTrivialType::GetObjectsCounter(), 0);

		{
			using Type = NotTrivialType;
			TypeLessVector vector = TypeLessVector::Create<Type>();

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

				componentIndex = vector.add(complex);
				ASSERT_NE(componentIndex, InvalidIndex);
			}

			const size_t removedComponentIndex = 4;
			vector.remove(removedComponentIndex);

			for (size_t i = 0; i < count; ++i)
			{
				if (i == removedComponentIndex)
					continue;

				auto resource = vector.get<Type>(i);
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

		ASSERT_EQ(NotTrivialType::GetObjectsCounter(), 0);
	}

	TEST_F(TypeLessVectorTests, IteratorsTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Sprite>();

		const size_t count = 6;
		for (size_t i = 0; i < count; ++i)
			vector.add(Sprite{ int(i) });
		
		vector.remove(0);
		vector.remove(2);
		vector.remove(5);

		const std::vector<int> expectedIds = { 1, 3, 4 };
		ASSERT_EQ(vector.getObjectsCount(), expectedIds.size());

		// TODO: Template iterators so we can skip reinterpret cast in the loop
		TypeLessVectorIterator it = vector.begin();
		TypeLessVectorIterator end = vector.end();
		size_t idIndex = 0;
		for (it; it != end; ++it)
		{
			void* rawPtr = *it;
			auto& sprite = *reinterpret_cast<Sprite*>(rawPtr);

			EXPECT_EQ(sprite.id, expectedIds[idIndex]);

			++idIndex;
		}

		// TODO: Add const iterators (See ecs::Query)
	}

	TEST_F(TypeLessVectorTests, ForRangeOverEmptyVectorTest)
	{
		TypeLessVector vector = TypeLessVector::Create<Sprite>();
		for ([[maybe_unused]] auto rawPtr : vector)
		{
			ASSERT_TRUE(false); // "We shouldn't enter this block";
		}
	}
}
