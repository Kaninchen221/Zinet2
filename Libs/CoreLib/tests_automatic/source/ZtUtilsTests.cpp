#pragma once

#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class UtilsTests : public ::testing::Test
    {
    protected:

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
}