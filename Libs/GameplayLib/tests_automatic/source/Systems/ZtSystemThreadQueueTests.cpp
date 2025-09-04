#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemThreadQueue.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemThreadQueueTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{

		}

		void TearDown() override
		{

		}

		class TestObject : public core::Object
		{
		public:
			bool touched = false;
			float value = 0.0f;
		};

		core::ObjectsStorage objectsStorage;
		SystemThreadQueue system;
	};

	TEST_F(SystemThreadQueueTests, PassTest)
	{
		auto objectHandle = objectsStorage.createObject<TestObject>("TestObject");
		ASSERT_TRUE(objectHandle);

		// Test state less lambda
		system.add(objectHandle, [](ObjectHandle<core::Object> objectHandle) 
		{ 
			ObjectHandle<TestObject> derived = objectHandle;
			derived->touched = true; 
		});
		ASSERT_FALSE(objectHandle->touched);
		ASSERT_EQ(system.getEventsCount(), 1);

		system.update();
		ASSERT_TRUE(objectHandle->touched);
		ASSERT_EQ(system.getEventsCount(), 0);

		// Test lambda with capture value
		const float expectedValue = 42.f;
		system.add(objectHandle, [value = expectedValue](ObjectHandle<core::Object> objectHandle)
		{
			ObjectHandle<TestObject> derived = objectHandle;
			derived->value = value;
		});
		ASSERT_EQ(system.getEventsCount(), 1);

		system.update();
		ASSERT_EQ(objectHandle->value, expectedValue);
		ASSERT_EQ(system.getEventsCount(), 0);
	}
}