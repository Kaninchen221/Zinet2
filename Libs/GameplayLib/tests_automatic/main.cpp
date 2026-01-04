#include <gtest/gtest.h>

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/TestUtils/ZtUtils.hpp"

int main(int argc, char* argv[]) 
{
    // TODO: Refactor setting callback inside other test targets
    zt::core::SimpleCallbackSink::SetCallback(zt::test_utils::FailCallback);
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}