#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeNodeEditorTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		NodeHandle<NodeEditor> node = CreateNode<NodeEditor>();
	};

	TEST_F(NodeNodeEditorTests, PassTest)
	{}
}