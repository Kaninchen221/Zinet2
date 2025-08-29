#pragma once

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace zt::core::tests
{

	class ObjectsStorageTests : public ::testing::Test
	{
	protected:

		ObjectsStorage objectsStorage;

		class TestObject : public core::Object
		{
		public:

			TestObject() {}
			~TestObject() {}

			bool onCreateInvoked = false;
			void onCreate() override
			{
				onCreateInvoked = true;
			}
			MOCK_METHOD(void, onDestroy, (), (override));

			/// Test if we could create ObjectHandle<TestObject in Node class
			using Nodes = std::vector<ObjectHandle<TestObject>>;
			Nodes nodes;

		};
	};

	TEST_F(ObjectsStorageTests, OnCreateOnDestroyTest)
	{
		ObjectHandle<TestObject> objHandle = objectsStorage.createObject<TestObject>("");
		ASSERT_TRUE(objHandle->onCreateInvoked);

		EXPECT_CALL(*objHandle.get(), onDestroy())
			.Times(1);

		objHandle.destroy();
	}

	TEST_F(ObjectsStorageTests, ComplexTest)
	{
		ObjectHandle<TestObject> node;
		ASSERT_FALSE(node.get());

		const std::string_view displayName = "Test Object";
		ObjectHandle<Object> objectHandle = objectsStorage.createObject<Object>(displayName);
		ASSERT_TRUE(objectHandle.get());
		ASSERT_TRUE(objectHandle.isValid());
		ASSERT_TRUE(objectHandle);
		ASSERT_EQ(objectHandle.getRefCount(), 1);

		{ /// Check weak handle doesn't affect ref count
			ObjectWeakHandle<Object> weakHandle = objectHandle.createWeakHandle();
			ASSERT_TRUE(weakHandle.get());
			ASSERT_TRUE(weakHandle.isValid());
			ASSERT_TRUE(weakHandle);
			ASSERT_EQ(weakHandle.getRefCount(), 1);

			weakHandle.invalidate();
			ASSERT_FALSE(weakHandle.get());
			ASSERT_FALSE(weakHandle.isValid());
			ASSERT_FALSE(weakHandle);
			ASSERT_EQ(weakHandle.getRefCount(), 0);
		}

		{ /// Check strong handle affect ref count
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

	TEST_F(ObjectsStorageTests, ReleaseTest)
	{
		const std::string_view displayName = "Test Object";
		ObjectHandle<Object> objectHandle = objectsStorage.createObject<Object>(displayName);

		ObjectRefCounter* refCounter = objectHandle.release();
		ASSERT_FALSE(objectHandle.get());
		ASSERT_FALSE(objectHandle.isValid());
		ASSERT_TRUE(refCounter);
	}

	TEST_F(ObjectsStorageTests, DerivedToBaseTest)
	{
		const std::string_view displayName = "Test Object";
		ObjectHandle<TestObject> node = objectsStorage.createObject<TestObject>(displayName);
		ASSERT_TRUE(node);

		ObjectHandle<Object> object = node;
		ASSERT_TRUE(object);
	}

	TEST_F(ObjectsStorageTests, DownCastTest)
	{
		auto testObject = objectsStorage.createObject<TestObject>("testObject");
		
		{
			ObjectHandle<Object> object = testObject;

			ASSERT_EQ(testObject.getRefCount(), 2);
			ASSERT_EQ(object.getRefCounter(), testObject.getRefCounter());
		}

		ASSERT_EQ(testObject.getRefCount(), 1);
	}
}