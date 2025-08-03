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

		ObjectHandle<NodeEditor> node = CreateObject<NodeEditor>("node");
	};

	TEST_F(NodeEditorTests, PassTest)
	{}
}