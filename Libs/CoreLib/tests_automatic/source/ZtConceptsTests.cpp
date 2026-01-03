#pragma once

#include "Zinet/Core/ZtConcepts.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

    class ConceptsTests : public ::testing::Test
    {
    protected:

        static_assert(STDContainer<std::vector<int>>);
        static_assert(NonSTDContainer<int>);

		static_assert(LikeSmartPointer<std::shared_ptr<int>>);
		static_assert(NotLikeSmartPointer<int>);

		static_assert(LikePair<std::pair<float, double>>);
		static_assert(NotLikePair<int>);
    };
}