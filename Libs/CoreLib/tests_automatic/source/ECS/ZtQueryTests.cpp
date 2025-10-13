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

		void SetUp() override;
	};

	TEST_F(ECSQueryTests, GetComponentsCountTest)
	{
		const Query<Sprite> query{ world };
		ASSERT_EQ(query.getComponentsCount(), 6);
	}

	TEST_F(ECSQueryTests, ForRangeBasedLoopTest)
	{
		size_t count = 0;
		for (const auto& component : Query<Sprite>{ world })
		{
			EXPECT_EQ(component.id, count);
			++count;
		}

		ASSERT_EQ(count, 6);
	}

	TEST_F(ECSQueryTests, ForRangeBasedLoopWithRemovedEntitiesTest)
	{
		world.remove(entities[0]); // First component
		world.remove(entities[3]); // Last component of the first segment
		world.remove(entities[5]); // Last component of the last segment

		const std::vector<size_t> expectedIndices = { 1, 2, 4 };

		size_t count = 0;
		for (const auto& component : Query<Sprite>{ world })
		{
			EXPECT_EQ(component.id, expectedIndices.at(count));
			++count;
		}

		ASSERT_EQ(count, 3);
	}

	void ECSQueryTests::SetUp()
	{
		entities.push_back(world.spawn(Sprite{ 0 }));
		entities.push_back(world.spawn(Sprite{ 1 }));
		entities.push_back(world.spawn(Sprite{ 2 }));
		entities.push_back(world.spawn(Sprite{ 3 }));
		entities.push_back(world.spawn(Sprite{ 4 }, Velocity{}));
		entities.push_back(world.spawn(Sprite{ 5 }, Position{}));
	}
}