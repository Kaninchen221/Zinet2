#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Gameplay/ZtEngine.hpp"

#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

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
			auto& assetsStorage = engineContext.getAssetsStorage();
			assetsStorage.registerAssetClass<core::AssetText>();
			assetsStorage.registerAssetClass<gameplay::AssetTexture>();
			assetsStorage.registerAssetClass<gameplay::AssetShader>();
			assetsStorage.registerAssetClass<gameplay::AssetSampler>();

			engineContext.addSystem<SystemImGui>("SystemImGui");
			engineContext.addSystem<SystemRenderer>("SystemRenderer");
			engineContext.addSystem<SystemSave>("SystemSave");
			engineContext.addSystem<SystemTickable>("SystemTickable");

			ASSERT_TRUE(engine.init());
			vulkan_renderer::ImGuiIntegration::SetStyle_Dark();

			auto& rootNode = engineContext.getRootNode();

			auto editorNode = CreateObject<NodeEditor>("Editor");
			rootNode->addChild(editorNode);

			auto systemImGui = engineContext.getSystem<SystemImGui>();
			ASSERT_TRUE(systemImGui);
			systemImGui->addNode(editorNode);

			auto child = CreateObject<Node>("Child");
			rootNode->addChild(child);
			
			auto childOfChild = CreateObject<Node>("Child of child");
			child->addChild(childOfChild);

			auto sprite = CreateObject<NodeSprite>("Sprite");
			sprite->transform.getPosition().z = 50;
			sprite->transform.getScale().x = 8;
			sprite->transform.getScale().y = 8;
			sprite->texture = assetsStorage.getAs<AssetTexture>("Content/Textures/image.png");
			sprite->texture->load(core::Paths::RootPath());
			rootNode->addChild(sprite);

			auto systemTickable = engineContext.getSystem<SystemTickable>();
			ASSERT_TRUE(systemTickable);

			systemTickable->addNode(sprite);

			auto systemRenderer = engineContext.getSystem<SystemRenderer>();
			ASSERT_TRUE(systemRenderer);

			systemRenderer->addNode(sprite);

			auto shaderVert = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
			ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
			systemRenderer->vertexShader = shaderVert;

			auto shaderFrag = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.frag");
			ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
			systemRenderer->fragmentShader = shaderFrag;

			auto nodeCamera = CreateObject<NodeCamera>("Camera");
			auto& camera = nodeCamera->getCamera();
			camera.setPosition(Vector3f(0.00001, 0, 150));
			camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			auto& window = engineContext.getWindow();
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