#pragma once

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class UtilsTests : public ::testing::Test
	{
	};

	TEST_F(UtilsTests, ContainsTest)
    {
        const std::vector<int> vector = { 1, 3, 4, 5, 6, 7, 8 };
		ASSERT_TRUE(Contains(vector, 3));
		ASSERT_FALSE(Contains(vector, 2));
	}

    TEST_F(UtilsTests, UniqueIDPerTypeTest)
    {
		struct Type1 {};
		struct Type2 {};

		const size_t type1ID = GetTypeID<Type1>();
		const size_t type2ID = GetTypeID<Type2>();

        ASSERT_NE(type1ID, type2ID);
	}

	TEST_F(UtilsTests, MakeTupleFromValueTest)
	{
		constexpr int expectedValue = 32;
		constexpr int expectedSize = 3;

		auto tuple = MakeTuple<int, expectedSize>(int(expectedValue));

		static_assert(std::tuple_size_v<decltype(tuple)> == expectedSize, "Invalid tuple size");

		EXPECT_EQ(std::get<0>(tuple), expectedValue);
		EXPECT_EQ(std::get<1>(tuple), expectedValue);
		EXPECT_EQ(std::get<2>(tuple), expectedValue);
	}

	TEST_F(UtilsTests, MakeTupleWithRefsTest)
	{
		struct Test
		{
			Test() noexcept = default;
			Test(const Test& other) noexcept = delete;
			Test(Test&& other) noexcept = delete;
			~Test() noexcept = default;

			Test& operator = (const Test& other) noexcept = delete;
			Test& operator = (Test&& other) noexcept = delete;
		};

		Test test;
		Test& ref = test;
		constexpr int expectedSize = 3;
		auto tuple = MakeTuple<Test&, expectedSize>(ref);

		static_assert(std::tuple_size_v<decltype(tuple)> == expectedSize, "Invalid tuple size");
	}

	TEST_F(UtilsTests, GetClassNameTest)
	{
		// Classes defined "normally" is enough for now
		std::string className = GetClassName<UtilsTests>();
		EXPECT_EQ(className, "UtilsTests");
	}
}