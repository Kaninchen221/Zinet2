#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

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

		core::ObjectsStorage objectsStorage;
		ObjectHandle<Node> node = objectsStorage.createObject<Node>("node");
	};

	TEST_F(NodeEditorTests, PassTest)
	{}
}