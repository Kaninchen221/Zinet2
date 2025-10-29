#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSQueryTests : public ::testing::Test
	{
	protected:

		World world;
		std::vector<Entity> entities;

		size_t expectedSpritesCount = 0;
		size_t expectedVelocitiesCount = 0;
		size_t expectedPositionsCount = 0;

		size_t expectedSpritesVelocitiesPairsCount = 0;

		void SetUp() override
		{
			fillWorldWithEntites();
		}

		void fillWorldWithEntites();
	};

	TEST_F(ECSQueryTests, GetComponentsCountTest)
	{
		const Query<Sprite> querySprites{ world };
		EXPECT_EQ(querySprites.getComponentsCount(), expectedSpritesCount);

		const Query<Velocity> queryVelocities{ world };
		EXPECT_EQ(queryVelocities.getComponentsCount(), expectedVelocitiesCount);

		const Query<Position> queryPositions{ world };
		EXPECT_EQ(queryPositions.getComponentsCount(), expectedPositionsCount);
	}

	TEST_F(ECSQueryTests, IteratorsSingleComponentTypeTest)
	{
		const Query<Sprite> query{ world };

		const QueryIterator<Sprite> begin = query.begin();
		const QueryIterator<Sprite> end = query.end();

		ASSERT_NE(begin, end);

		auto it = begin;
		size_t index = 0;
		for (index; index < expectedSpritesCount; ++index)
		{
			ASSERT_NE(it, end);

			auto [sprite] = *it;
			ASSERT_TRUE(sprite);
			EXPECT_EQ(sprite->id, index);
			++it;
		}

		++it;
		ASSERT_EQ(it, end);
	}

	TEST_F(ECSQueryTests, IteratorsMultipleComponentsTypesTest)
	{
		const Query<Sprite, Velocity> query{ world };

		const QueryIterator<Sprite, Velocity> begin = query.begin();
		const QueryIterator<Sprite, Velocity> end = query.end();

		ASSERT_NE(begin, end);

		for (auto it = begin; it != end; ++it)
		{
			auto [sprite, velocity] = *it;
			ASSERT_TRUE(sprite);
			ASSERT_TRUE(velocity);
		}
	}

	TEST_F(ECSQueryTests, ForRangeLoopTest)
	{
		const Query<Sprite> query{ world };
		size_t counter = 0;

		for (auto [sprite] : query)
		{
			++counter;
		}

		ASSERT_EQ(counter, expectedSpritesCount);
	}
}

namespace zt::core::ecs::tests
{
	void ECSQueryTests::fillWorldWithEntites()
	{
		entities.push_back(world.spawn(Sprite{ 0 }));
		entities.push_back(world.spawn(Sprite{ 1 }));
		entities.push_back(world.spawn(Sprite{ 2 }));
		entities.push_back(world.spawn(Sprite{ -1 }));
		world.remove(entities.back());
		entities.pop_back();

		entities.push_back(world.spawn(Sprite{ 3 }));
		entities.push_back(world.spawn(Sprite{ 4 }));
		entities.push_back(world.spawn(Sprite{ -1 }, Velocity{ -1, -1 }));
		world.remove(entities.back());
		entities.pop_back();

		entities.push_back(world.spawn(Sprite{ 5 }, Velocity{ 5, 5 }));
		entities.push_back(world.spawn(Sprite{ 6 }, Velocity{ 6, 6 }));
		entities.push_back(world.spawn(Sprite{ 7 }, Velocity{ 7, 7 }));
		entities.push_back(world.spawn(Velocity{ -1, -1 })); // Impostor
		entities.push_back(world.spawn(Sprite{ 8 }, Velocity{ 8, 8 }));
		entities.push_back(world.spawn(Sprite{ -1 }, Position{ -1, -1 }));
		world.remove(entities.back());
		entities.pop_back();

		entities.push_back(world.spawn(Sprite{ 9 }, Position{ 9, 9 }));
		entities.push_back(world.spawn(Sprite{ 10 }, Position{ 10, 10 }));
		entities.push_back(world.spawn(Sprite{ -1 }, Position{ -1, -1 }));
		world.remove(entities.back());
		entities.pop_back();

		expectedSpritesCount = 11;
		expectedVelocitiesCount = 5;
		expectedPositionsCount = 2;

		expectedSpritesVelocitiesPairsCount = 4;
	}

	TEST(ECSQueryTest, QueryComponentsThatDontExistTest)
	{
		World world;
		Query<Sprite> query{ world };
		for (auto [sprite] : query)
		{
			ASSERT_TRUE(false);
		}

		const auto& constQuery = query;
		for (auto [sprite] : constQuery)
		{
			ASSERT_TRUE(false);
		}
	}
}