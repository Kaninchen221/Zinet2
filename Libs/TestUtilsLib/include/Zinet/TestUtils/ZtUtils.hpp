#pragma once

#include "Zinet/TestUtils/ZtTestUtilsConfig.hpp"

#include <type_traits>

#include <gtest/gtest.h>

namespace zt::test_utils
{  
	ZINET_TEST_UTILS_API void Placeholder();

	inline static auto FailCallback = []() { FAIL() << "Some logger log error, warning or critical"; };
}