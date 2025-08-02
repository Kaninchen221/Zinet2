#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemSaveTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{

		}

		void TearDown() override
		{

		}

		class NodeTest : public Node
		{
		public:

			NodeTest(std::string_view name) : Node{ name } {}

			bool serialize(core::JsonArchive& archive) ZINET_API_POST 
			{
				archive.serialize("Value", value);

				return true;
			}

			int value = 0;

		};

	};

	TEST_F(SystemSaveTests, PassTest)
	{
		SystemSave system;
		system.setSaveFolderPath(core::Paths::CurrentProjectRootPath() / "Saves");

		auto node = CreateNode<NodeTest>();
		system.addNode(node);

		node->value = 404;

		ASSERT_TRUE(system.createArchiveFromNodes());
		ASSERT_TRUE(system.putArchiveIntoFile());

		system.clearCurrentBuffer();

		ASSERT_TRUE(system.putFileIntoArchive());
	}
}