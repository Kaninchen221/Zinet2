#include <gtest/gtest.h>

#include "Zinet/Core/ZtLogger.hpp"

int main(int argc, char* argv[]) 
{
    auto callback = []() { FAIL() << "Some logger log error, warning or critical"; };
    zt::core::CustomSink<>::SetFailTestCallback(callback);

    ::testing::InitGoogleTest(&argc, argv);
    auto runAllTestsResult = RUN_ALL_TESTS();

    return runAllTestsResult;
}