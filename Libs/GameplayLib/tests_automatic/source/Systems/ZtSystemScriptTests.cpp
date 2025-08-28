#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemScript.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemScriptTests : public ::testing::Test
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

		class TestNode : public Node
		{
		public:

		};

		core::ObjectsStorage objectsStorage;
		SystemScript system;
	};

	TEST_F(SystemScriptTests, PassTest)
	{
		auto node = objectsStorage.createObject<TestNode>("node");
		system.addNode(node);

		system.update();
	}
}