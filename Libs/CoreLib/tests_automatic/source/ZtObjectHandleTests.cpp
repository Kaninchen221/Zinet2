#pragma once

#include "Zinet/Core/ZtObjectHandle.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ObjectHandleTests : public ::testing::Test
	{
	protected:

		ObjectHandleTests()
		{
		}

		~ObjectHandleTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		ObjectHandle<Object> objectHandle;
	};

	TEST_F(ObjectHandleTests, Test)
	{
	}
}