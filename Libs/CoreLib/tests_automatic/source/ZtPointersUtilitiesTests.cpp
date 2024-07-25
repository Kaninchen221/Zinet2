#pragma once

#include "Zinet/Core/ZtPointersUtilities.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class PointersUtilitiesTests : public ::testing::Test
	{
	protected:

		PointersUtilitiesTests()
		{
		}

		~PointersUtilitiesTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		class ObjectChild : public Object
		{
			std::unique_ptr<ObjectBase> createCopy() const override { return nullptr; }
		};

	};

	TEST_F(PointersUtilitiesTests, DynamicCastUniquePtr)
	{
		Object* rawObject = new ObjectChild{};
		std::unique_ptr<Object> object = std::unique_ptr<Object>(rawObject);
		std::unique_ptr<ObjectChild> objectCasted = PointersUtilities::DynamicCastUniquePtr<Object, ObjectChild>(object);

		ASSERT_FALSE(object);
		ASSERT_TRUE(objectCasted);
		ASSERT_EQ(objectCasted.get(), rawObject);
	}

	TEST_F(PointersUtilitiesTests, StaticCastUniquePtr)
	{
		Object* rawObject = new ObjectChild{};
		std::unique_ptr<Object> object = std::unique_ptr<Object>(rawObject);
		std::unique_ptr<ObjectChild> objectCasted = PointersUtilities::StaticCastUniquePtr<Object, ObjectChild>(object);

		ASSERT_FALSE(object);
		ASSERT_TRUE(objectCasted);
		ASSERT_EQ(objectCasted.get(), rawObject);
	}
}