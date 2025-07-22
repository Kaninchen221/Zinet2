#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Gameplay/ZtEngine.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

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
			auto& engineContext = EngineContext::Get();
			engineContext.assetsStorage.registerAssetClass<core::assets::AssetText>();
			engineContext.assetsStorage.registerAssetClass<gameplay::assets::AssetTexture>();

			ASSERT_TRUE(engine.init());
			vulkan_renderer::ImGuiIntegration::SetStyle_Dark();

			auto& rootNode = engineContext.rootNode;

			auto editorNode = CreateNode<NodeEditor>();
			editorNode->setName("Editor");
			rootNode->addChild(editorNode);
			engineContext.systemImGui.addNode(editorNode);

			auto child = CreateNode();
			child->setName("Child");
			rootNode->addChild(child);

			auto childOfChild = CreateNode();
			childOfChild->setName("Child of child");
			child->addChild(childOfChild);

			auto sprite = CreateNode<NodeSprite>("Sprite");
			sprite->texture = engineContext.assetsStorage.getAs<assets::AssetTexture>("Content/Textures/image.png");
			rootNode->addChild(sprite);
			engineContext.systemRenderer.addNode(sprite);
		}

		void TearDown() override
		{
			engine.deinit();
		}

		Engine engine;
	};

	TEST_F(EngineTests, PassTest)
	{
		while (engine.shouldLoop())
		{
			engine.loop();
		}
	}
}