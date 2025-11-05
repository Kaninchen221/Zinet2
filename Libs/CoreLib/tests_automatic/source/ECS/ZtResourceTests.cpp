#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSResourceTests : public ::testing::Test
	{
	protected:

		World world;

		static_assert(!std::is_default_constructible_v<Resource<int>>);
	};

	TEST_F(ECSResourceTests, Test)
	{
		const int expected = 50;
		world.addResource(int(expected));

		Resource<int> resource{ world };
		ASSERT_TRUE(resource);
		ASSERT_TRUE(resource.operator->());
		
		const int actual = *resource.get();
		ASSERT_EQ(expected, actual);
	}
}