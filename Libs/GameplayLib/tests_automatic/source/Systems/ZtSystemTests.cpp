#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(system.init(engineContext));
		}

		void TearDown() override
		{
			system.deinit();
		}

		EngineContext engineContext;

		using NodeBase = Node;
		System<Node> system;
	};

	TEST_F(SystemTests, NodesTest)
	{
		auto node = CreateNode();
		system.addNode(node);
		ASSERT_EQ(node.use_count(), 1);

		ASSERT_EQ(system.getNodes().size(), 1);
	}
}