#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeTest : public Node
	{
	public:

		bool serialize(core::JsonArchive& archive) override ZINET_API_POST
		{
			archive.serialize("Value", value);

			return true;
		}

		bool deserialize(core::JsonArchive& archive) override ZINET_API_POST
		{
			archive.deserialize("Value", value);

			return true;
		}

		int value = 0;

	};

	class SystemSaveTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			engineContext.addSystem<SystemSave>("SystemSave");

			engineContext.classRegistry.registerClass<NodeTest>("NodeTestClassName");

			engineContext.init();
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
	};

	TEST_F(SystemSaveTests, PassTest)
	{
		SystemSave system;
		system.setSaveFolderPath(core::Paths::CurrentProjectRootPath() / "Saves");

		auto node = CreateObject<NodeTest>("NodeTestName");
		system.addNode(node);

		node->value = 404;

		ASSERT_TRUE(system.createArchiveFromNodes());
		ASSERT_TRUE(system.putArchiveIntoFile());

		system.clearCurrentBuffer();

		ASSERT_TRUE(system.putFileIntoArchive());
		ASSERT_TRUE(system.recreateNodesFromArchive());
	}
}