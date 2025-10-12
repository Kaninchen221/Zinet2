#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include <ranges>

namespace zt::core::ecs::tests
{
	class ECSTypeLessVectorTests : public ::testing::Test
	{
	protected:

		struct MockResourceComplex : public ResourceComplex
		{
			//MOCK_METHOD(void, onCreated, (), (override));
			MOCK_METHOD(void, onDestroyed, (), (override));

			MockResourceComplex& operator = (const MockResourceComplex& other)
			{
				name = other.name;
				data = other.data;
				return *this;
			}
		};
	};

	TEST_F(ECSTypeLessVectorTests, CreateTest)
	{
		const Position expectedPosition{ 1.0f, 2.0f };
		TypeLessVector components = TypeLessVector::Create<Position>();
		
		ASSERT_EQ(components.getTypeID(), GetTypeID<Position>());
		ASSERT_EQ(components.getSize(), 0);
	}

	TEST_F(ECSTypeLessVectorTests, GetFromInvalidIndexTest)
	{
		TypeLessVector components = TypeLessVector::Create<Position>();
		auto sprite = components.get<Sprite>(0);

		ASSERT_FALSE(sprite);
	}

	// TODO: The component that we pass to the type less vector must be moved to the internal storage so it will loose "ownership to the data"
	// We must manualy invoke destructor for the components in the internal storage when we remove a component and when we destroy the entire vector
	// Reason: Components and resources could have a vector or other storage that can point to some space in the memory
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

	TEST_F(ECSTypeLessVectorTests, AddResourceComplexTest)
	{
		using Type = ResourceComplex;
		TypeLessVector components = TypeLessVector::Create<Type>();

		std::string expectedName = "TestName";
		std::initializer_list<int32_t> expectedData =
		{ { 0 }, { 1 }, { 2 }, { 3 } };

		size_t componentIndex = InvalidIndex;
		{ // TODO: Handle situation when resource has complex data
			Type complex;
			complex.name = expectedName;
			complex.data = expectedData;

			componentIndex = components.add(complex);
			ASSERT_NE(componentIndex, InvalidIndex);
		}

		auto resource = components.get<Type>(componentIndex);
		ASSERT_TRUE(resource);

		EXPECT_EQ(expectedName, resource->name);

		ASSERT_EQ(expectedData.size(), resource->data.size());
		for (const auto& [expected, actual] : std::views::zip(expectedData, resource->data))
		{
			EXPECT_EQ(expected, actual);
		}
	}
}