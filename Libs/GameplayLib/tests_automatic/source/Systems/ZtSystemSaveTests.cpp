#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeTest : public Node
	{
	public:

		NodeTest() : Node{} { setSaveable(true); }

		virtual std::string getClassName() const { return "zt::gameplay::tests::NodeTest"; }

		ObjectPtr createCopy() const override { return std::make_unique<NodeTest>(*this); }

		bool serialize(core::JsonArchive& archive) override
		{
			Node::serialize(archive);

			archive.serialize("Value", value);
			archive.serialize("Value2", value2);

			return true;
		}

		bool deserialize(core::JsonArchive& archive) override
		{
			Node::deserialize(archive);

			archive.deserialize("Value", value);
			archive.deserialize("Value2", value2);

			return true;
		}

		int value = 0;
		int value2 = 0;

	};

	class SystemSaveTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			engineContext.addSystem<SystemSave>("SystemSave");

			engineContext.getClassRegistry().registerClass<Node>();
			engineContext.getClassRegistry().registerClass<NodeTest>();

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

		auto& rootNode = engineContext.getRootNode();

		const int nodeTest1Value = 404;
		const int nodeTest2Value = 68;
		const int nodeTest3Value2 = 2138;

		{
			auto node = CreateObject<NodeTest>("NodeTest1");
			rootNode->addChild(node);
			node->value = nodeTest1Value;

			auto childNode = CreateObject<NodeTest>("NodeTest3");
			node->addChild(childNode);
			childNode->value2 = nodeTest3Value2;
		}

		{
			auto node = CreateObject<NodeTest>("NodeTest2");
			rootNode->addChild(node);
			node->value = nodeTest2Value;
		}

		ASSERT_TRUE(system.createArchiveFromNodes(rootNode));
		ASSERT_TRUE(system.putArchiveIntoFile());

		rootNode = {};
		system.clearCurrentBuffer();

		// TODO: Rewrite this
		//ASSERT_TRUE(system.putFileIntoArchive());
		//rootNode = system.recreateNodesFromArchive();
		//ASSERT_TRUE(engineContext.rootNode);
		//
		//auto& children = rootNode->getChildren();
		//ASSERT_EQ(children.size(), 2);
		//
		//{
		//	auto nodeTest1 = std::dynamic_pointer_cast<NodeTest>(children[0]);
		//	ASSERT_TRUE(nodeTest1);
		//	ASSERT_EQ(nodeTest1->value, nodeTest1Value);
		//
		//	auto& nodeTest1Children = nodeTest1->getChildren();
		//	ASSERT_EQ(nodeTest1Children.size(), 1);
		//
		//	auto nodeTest3 = std::dynamic_pointer_cast<NodeTest>(nodeTest1Children[0]);
		//	ASSERT_TRUE(nodeTest3);
		//	ASSERT_EQ(nodeTest3->value2, nodeTest3Value2);
		//}
		//
		//{
		//	auto nodeTest2 = std::dynamic_pointer_cast<NodeTest>(children[1]);
		//	ASSERT_TRUE(nodeTest2);
		//	ASSERT_EQ(nodeTest2->value, nodeTest2Value);
		//}

	}
}