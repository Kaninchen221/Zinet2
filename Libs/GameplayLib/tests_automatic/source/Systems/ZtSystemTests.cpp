#pragma once

#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemTests : public ::testing::Test
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

		using NodeBase = Node;
		System system{ "System" };

		static_assert(!std::is_default_constructible_v<System>);
	};

	TEST_F(SystemTests, NameTest)
	{
		ASSERT_EQ(system.getName(), "System");
	}

	TEST_F(SystemTests, NodesTest)
	{
		auto node = CreateNode();
		system.addNode(node);
		ASSERT_EQ(node.use_count(), 1);

		ASSERT_EQ(system.getNodes().size(), 1);
	}
}