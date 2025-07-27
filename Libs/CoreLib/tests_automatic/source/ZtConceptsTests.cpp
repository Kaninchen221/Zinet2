#pragma once

#include "Zinet/Core/ZtConcepts.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class ConceptsTests : public ::testing::Test
    {
    protected:

        static_assert(STDContainer<std::vector<int>>);
        static_assert(NotSTDContainer<int>);

    };
}