#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Gameplay/ZtEngine.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

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
			auto& assetsStorage = engineContext.assetsStorage;
			assetsStorage.registerAssetClass<core::assets::AssetText>();
			assetsStorage.registerAssetClass<gameplay::assets::AssetTexture>();
			assetsStorage.registerAssetClass<gameplay::assets::AssetShader>();
			assetsStorage.registerAssetClass<gameplay::assets::AssetSampler>();

			engineContext.addSystem<SystemImGui>();
			engineContext.addSystem<SystemRenderer>();

			ASSERT_TRUE(engine.init());
			vulkan_renderer::ImGuiIntegration::SetStyle_Dark();

			auto& rootNode = engineContext.rootNode;

			auto editorNode = CreateNode<NodeEditor>();
			editorNode->setName("Editor");
			rootNode->addChild(editorNode);

			auto systemImGui = engineContext.getSystem<SystemImGui>();
			ASSERT_TRUE(systemImGui);
			systemImGui->addNode(editorNode);

			auto child = CreateNode();
			child->setName("Child");
			rootNode->addChild(child);

			auto childOfChild = CreateNode();
			childOfChild->setName("Child of child");
			child->addChild(childOfChild);

			auto sprite = CreateNode<NodeSprite>("Sprite");
			sprite->texture = assetsStorage.getAs<assets::AssetTexture>("Content/Textures/image.png");
			sprite->texture->load(core::Paths::RootPath());
			rootNode->addChild(sprite);

			auto systemRenderer = engineContext.getSystem<SystemRenderer>();
			ASSERT_TRUE(systemRenderer);

			auto shaderVert = assetsStorage.getAs<assets::AssetShader>("Content/Shaders/shader.vert");
			ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
			systemRenderer->vertexShader = shaderVert;

			auto shaderFrag = assetsStorage.getAs<assets::AssetShader>("Content/Shaders/shader.frag");
			ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
			systemRenderer->fragmentShader = shaderFrag;
			systemRenderer->addNode(sprite);

			auto nodeCamera = CreateNode<NodeCamera>("Camera");
			auto& camera = nodeCamera->getCamera();
			camera.setPosition(Vector3f(0.00001, 0, 150));
			camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			auto& window = engineContext.window;
			auto windowSize = window.getSize();
			camera.setFieldOfView(45.f);
			camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
			camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });

			systemRenderer->setCameraNode(nodeCamera);
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