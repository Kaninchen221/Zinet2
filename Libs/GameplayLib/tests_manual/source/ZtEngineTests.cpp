#pragma once

#include "Zinet/Gameplay/ZtEngine.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class EngineTests : public ::testing::Test
	{
	protected:

		EngineTests()
		{
		}

		~EngineTests() override
		{
		}

		void SetUp() override
		{
			ASSERT_TRUE(engine.init());
			EngineContext::Get().systemImGui.addNode(editorNode);
		}

		void TearDown() override
		{
			engine.deinit();
		}

		Engine engine;
		NodeHandle<NodeEditor> editorNode = CreateNode<NodeEditor>();
	};

	TEST_F(EngineTests, PassTest)
	{
		while (engine.shouldLoop())
		{
			engine.loop();
		}
	}
}