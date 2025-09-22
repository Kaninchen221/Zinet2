#pragma once

#include "Zinet/Core/ZtObjectRefCounter.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ObjectRefCounterTests : public ::testing::Test
	{
	protected:

		ObjectRefCounterTests()
		{
		}

		~ObjectRefCounterTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		ObjectRefCounter objectRefCounter;
	};

	TEST_F(ObjectRefCounterTests, Test)
	{
	}
}