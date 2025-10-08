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

		void SetUp() override;
	};

	TEST_F(ECSQueryTests, Test)
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

	void ECSQueryTests::SetUp()
	{
		world.spawn(Sprite{ 0 });
		world.spawn(Sprite{ 1 });
		world.spawn(Sprite{ 2 });
		world.spawn(Sprite{ 3 });
		world.spawn(Sprite{ 4 }, Velocity{});
		world.spawn(Sprite{ 5 }, Position{});
	}
}