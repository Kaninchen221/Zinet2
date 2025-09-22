#pragma once

#include "Zinet/Core/ZtObjectHandle.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ObjectHandleTests : public ::testing::Test
	{
	protected:

		ObjectRefCounter objectRefCounter;
		ObjectHandle<Object> objectHandle;

		class TestObject : public Object
		{
		};
	};

	TEST_F(ObjectHandleTests, PassTest)
	{
		ASSERT_FALSE(objectHandle.isValid());
		ASSERT_FALSE(objectHandle.operator bool());

		objectRefCounter.create<Object>("Test Object");
		ASSERT_TRUE(objectRefCounter.isValid());

		objectHandle = ObjectHandle<Object>{ &objectRefCounter };

		ASSERT_TRUE(objectHandle.isValid());
		ASSERT_TRUE(objectHandle.operator bool());
		ASSERT_TRUE(objectHandle.get());
		ASSERT_TRUE(objectHandle.operator ->());

		ASSERT_EQ(objectHandle.getRefCounter(), &objectRefCounter);
		ASSERT_EQ(objectRefCounter.getRefCount(), 1);

		{
			auto objectHandle2 = objectHandle.createHandle();
			auto objectWeakHandle = objectHandle.createWeakHandle();
			ASSERT_TRUE(objectHandle2.isValid());
			ASSERT_EQ(objectRefCounter.getRefCount(), 2);
		}
		ASSERT_EQ(objectRefCounter.getRefCount(), 1);

		objectHandle.destroy();
		ASSERT_FALSE(objectHandle.isValid());
		ASSERT_FALSE(objectRefCounter.isValid());
	}

	TEST_F(ObjectHandleTests, CastTest)
	{
		objectRefCounter.create<TestObject>("Test Object");
		ASSERT_TRUE(objectRefCounter.isValid());

		objectHandle = ObjectHandle<Object>{ &objectRefCounter };
		ASSERT_TRUE(objectHandle.isValid());

		ASSERT_EQ(objectRefCounter.getRefCount(), 1);

		ObjectHandle<TestObject> castedHandle = objectHandle.castTo<TestObject, true>();
		ASSERT_TRUE(castedHandle.isValid());

		ASSERT_EQ(objectRefCounter.getRefCount(), 2);

		castedHandle.invalidate();
		ASSERT_EQ(objectRefCounter.getRefCount(), 1);
	}
}