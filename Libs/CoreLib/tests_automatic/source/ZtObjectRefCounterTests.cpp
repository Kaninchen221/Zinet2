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
		ASSERT_FALSE(objectRefCounter.isValid());
		//ASSERT_FALSE(objectRefCounter.get()); // Contains sanity check when contains an nullptr
		//ASSERT_FALSE(objectRefCounter.operator->()); // Contains sanity check when contains an nullptr
		ASSERT_FALSE(objectRefCounter.operator bool());

		auto expectedName = std::string_view("Test Object");
		objectRefCounter.create<Object>(expectedName);

		ASSERT_TRUE(objectRefCounter.isValid());
		ASSERT_TRUE(objectRefCounter.get());
		ASSERT_TRUE(objectRefCounter.operator->());
		ASSERT_TRUE(objectRefCounter.operator bool());

		ASSERT_EQ(objectRefCounter->getDisplayName(), expectedName);

		ASSERT_EQ(objectRefCounter.getRefCount(), 0);

		objectRefCounter.increment();
		ASSERT_EQ(objectRefCounter.getRefCount(), 1);

		objectRefCounter.decrement();
		ASSERT_EQ(objectRefCounter.getRefCount(), 0);

		objectRefCounter.increment();
		objectRefCounter.increment();
		objectRefCounter.increment();
		ASSERT_EQ(objectRefCounter.getRefCount(), 3);

		objectRefCounter.destroy();
		ASSERT_FALSE(objectRefCounter.isValid());
		ASSERT_EQ(objectRefCounter.getRefCount(), 0);
	}
}