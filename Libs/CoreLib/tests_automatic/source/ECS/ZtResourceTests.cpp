#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtTypeLessVector.hpp"

#include "Zinet/Core/ECS/ZtResource.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSResourceTests : public ::testing::Test
	{
	protected:

		static_assert(!std::is_default_constructible_v<Resource<int>>);
	};

	TEST_F(ECSResourceTests, Test)
	{
		World world;

		const int expected = 50;
		world.addResource(int(expected));

		Resource<int> resource{ world };
		ASSERT_TRUE(resource);
		ASSERT_TRUE(resource.operator->());
		ASSERT_TRUE(resource.operator*());
		
		const int actual = *resource.get();
		ASSERT_EQ(expected, actual);
	}

	TEST_F(ECSResourceTests, ConstTest)
	{
		World world;
		const Resource<int> resource{ world };

		static_assert(std::is_same_v<decltype(resource.get()), int const*>, "const Resource<T>::get must return a pointer to const variable");
		static_assert(std::is_same_v<decltype(resource.operator ->()), int const*>, "const Resource<T>::operator -> must return a pointer to const variable");
	}

	TEST_F(ECSResourceTests, ConstWorldTest)
	{
		World world;
		const int expectedInt = 40;
		ASSERT_TRUE(world.addResource(int(expectedInt)));

		const World& constWorld = world;
		ConstResource<int> resource{ constWorld };
		ASSERT_TRUE(resource);

		EXPECT_EQ(expectedInt, *resource.get());
	}
}