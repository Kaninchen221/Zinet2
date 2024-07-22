#pragma once

#include "Zinet/Core/ZtObject.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ObjectTests : public ::testing::Test
	{
	protected:

		ObjectTests()
		{
		}

		~ObjectTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(ObjectTests, Pass)
	{

	}

}