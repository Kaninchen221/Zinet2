#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemTickableTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(system.init());
		}

		void TearDown() override
		{
			system.deinit();
		}

		SystemTickable system;
	};

	class TestNode : public Node
	{
	public:
		void update(float deltaTime) override
		{
			updated = true;
			timeElapsed += deltaTime;
		}
		bool updated{ false };
		float timeElapsed = 0.f;
	};

	TEST_F(SystemTickableTests, PassTest)
	{
		core::ObjectsStorage objectsStorage;
		auto node = objectsStorage.createObject<TestNode>("node");
		system.addNode(node);

		system.update();
		EXPECT_TRUE(node->updated);
		EXPECT_TRUE(node->timeElapsed > 0.f);
	}
}