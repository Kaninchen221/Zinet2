#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeEditorTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		NodeHandle<NodeEditor> node = CreateNode<NodeEditor>();

		static_assert(!std::is_default_constructible_v<NodeEditor>);
		static_assert(std::is_constructible_v<NodeEditor, const NodeNameView&>);
	};

	TEST_F(NodeEditorTests, PassTest)
	{}
}