#pragma once

#include "Zinet/Core/ZtObjectBase.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class ObjectBaseTests : public ::testing::Test
    {
    protected:

        ObjectBaseTests()
        {
        }

        ~ObjectBaseTests() override 
        {
        }

        void SetUp() override 
        {
        }

        void TearDown() override 
        {
        }

    };

    TEST_F(ObjectBaseTests, PassTest)
    {

    }
}