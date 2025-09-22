#pragma once

#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class FunctionTests : public ::testing::Test
    {
    protected:

        FunctionTests()
        {
        }

        ~FunctionTests() override
        {
        }

        void SetUp() override
        {
        }

        void TearDown() override
        {
        }

        static int MarkAndReturnI(ObjectHandle<Object>&, int i) { fooInvoked = true; return i; }

        inline static bool fooInvoked = false;
    };

	TEST_F(FunctionTests, Test)
	{
		auto lambda = [](ObjectHandle<Object>&, int) -> int { return {}; };
		Function<int, ObjectHandle<Object>&, int> function{ lambda };

		ASSERT_EQ(function.getInternalFunction(), lambda);
		ASSERT_TRUE(function.isValid());
		ASSERT_TRUE(function.operator bool());

		function.invalidate();

		ASSERT_FALSE(function.isValid());

		function = MarkAndReturnI;
		ASSERT_TRUE(function.isValid());

		ObjectHandle<Object> param;
		const int expectedValue = 0;

		const auto& constFunction = function;
		const int actualValue = constFunction.invoke(param, expectedValue);
		ASSERT_EQ(expectedValue, actualValue);
	}

}