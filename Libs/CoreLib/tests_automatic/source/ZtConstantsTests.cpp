#pragma once

#include "Zinet/Core/ZtConstants.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class ConstantsTests : public ::testing::Test
    {
    protected:

        static_assert(InvalidIndex == -1);
	};

	template<class IsStaticV = Static::Yes>
	struct TestStruct
	{
		using IsStatic = IsStaticV;
	};

    TEST_F(ConstantsTests, StaticTest)
    {
		static_assert(Static::Yes{} == true);
		static_assert(Static::No{} == false);

		static_assert(TestStruct<Static::Yes>::IsStatic{} == Static::Yes{});
		static_assert(TestStruct<Static::No>::IsStatic{} == Static::No{});
	}
}