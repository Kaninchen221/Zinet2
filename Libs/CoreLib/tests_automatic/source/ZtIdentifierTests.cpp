#pragma once

#include "Zinet/Core/ZtIdentifier.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::core::tests
{

    class IdentifierTests : public ::testing::Test
    {
    protected:

    };

    TEST_F(IdentifierTests, CopyTest)
    {
        size_t expectedSecondIDUnderlyingNumber = 23;
        Identifier firstID(expectedSecondIDUnderlyingNumber);
        Identifier secondID(firstID);

        size_t actualSecondIDUnderlyingNumber = secondID.getUnderlyingNumber();
        ASSERT_EQ(actualSecondIDUnderlyingNumber, expectedSecondIDUnderlyingNumber);

        size_t actualFirstIDUnderlyingNumber = firstID.getUnderlyingNumber();
        ASSERT_EQ(actualFirstIDUnderlyingNumber, Identifier::Invalid);
    }

    TEST_F(IdentifierTests, MoveTest)
    {
        size_t expectedSecondIDUnderlyingNumber = 23;
        Identifier firstID(expectedSecondIDUnderlyingNumber);
        Identifier secondID(std::move(firstID));

        size_t actualSecondIDUnderlyingNumber = secondID.getUnderlyingNumber();
        ASSERT_EQ(actualSecondIDUnderlyingNumber, expectedSecondIDUnderlyingNumber);

        size_t actualFirstIDUnderlyingNumber = firstID.getUnderlyingNumber();
        ASSERT_EQ(actualFirstIDUnderlyingNumber, Identifier::Invalid);
    }

    TEST_F(IdentifierTests, GetUnderlyingNumberTest)
    {
        size_t expectedUnderlyingNumber = 45;
        Identifier uniqueIdentifier(expectedUnderlyingNumber);

        size_t actualUnderlyingNumber = uniqueIdentifier.getUnderlyingNumber();

        ASSERT_EQ(actualUnderlyingNumber, expectedUnderlyingNumber);
    }

}