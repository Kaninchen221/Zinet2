#pragma once

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ObjectsStorageTests : public ::testing::Test
	{
	protected:

		ObjectsStorage objectsStorage;

	};

	TEST_F(ObjectsStorageTests, SimpleTest)
	{
		const std::string_view displayName = "Test Object";
		ObjectHandle<Object> objectHandle = objectsStorage.createObject<Object>(displayName);
		ASSERT_TRUE(objectHandle.get());
		ASSERT_TRUE(objectHandle.isValid());
		ASSERT_TRUE(objectHandle);
		ASSERT_EQ(objectHandle.getRefCount(), 1);

		{
			auto secondHandle = objectHandle;
			ASSERT_EQ(secondHandle.get(), objectHandle.get());

			ASSERT_EQ(objectHandle.getRefCount(), 2);
			ASSERT_EQ(secondHandle.getRefCount(), 2);
		}

		objectHandle.invalidate();
		ASSERT_FALSE(objectHandle.get());
		ASSERT_FALSE(objectHandle.isValid());
		ASSERT_FALSE(objectHandle);
		ASSERT_EQ(objectHandle.getRefCount(), 0);
	}
}