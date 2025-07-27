#include <gtest/gtest.h>

#include "Zinet/Core/ZtLogger.hpp"

int main(int argc, char* argv[])
{
	auto callback = []() { FAIL() << "Some logger log error, warning or critical"; };
	zt::core::CustomSink::SetCallback(callback);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}