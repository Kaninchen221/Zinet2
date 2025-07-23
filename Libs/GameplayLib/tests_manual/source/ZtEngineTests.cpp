#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Gameplay/ZtEngine.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/VulkanRenderer/Assets/ZtAssetShader.hpp"

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
			engineContext.assetsStorage.registerAssetClass<vulkan_renderer::assets::AssetShader>();

			engineContext.addSystem<SystemImGui>();
			engineContext.addSystem<SystemRenderer>();

			ASSERT_TRUE(engine.init());
			vulkan_renderer::ImGuiIntegration::SetStyle_Dark();

			auto& rootNode = engineContext.rootNode;

			auto editorNode = CreateNode<NodeEditor>();
			editorNode->setName("Editor");
			rootNode->addChild(editorNode);
			engineContext.getSystem<SystemImGui>().addNode(editorNode);

			auto child = CreateNode();
			child->setName("Child");
			rootNode->addChild(child);

			auto childOfChild = CreateNode();
			childOfChild->setName("Child of child");
			child->addChild(childOfChild);

			auto sprite = CreateNode<NodeSprite>("Sprite");
			sprite->texture = engineContext.assetsStorage.getAs<assets::AssetTexture>("Content/Textures/image.png");
			rootNode->addChild(sprite);
			engineContext.getSystem<SystemRenderer>().addNode(sprite);
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