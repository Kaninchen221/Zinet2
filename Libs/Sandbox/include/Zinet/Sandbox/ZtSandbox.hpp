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
#include "Zinet/Gameplay/Systems/ZtSystemThreadQueue.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#define ASSERT_TRUE(EXPR) EXPR

namespace zt::sandbox
{
	class Sandbox
	{
	public:

		void start()
		{
			using namespace zt;
			using namespace zt::gameplay;

			Engine engine;

			auto& engineContext = EngineContext::Get();
			auto& assetsStorage = engineContext.getAssetsStorage();
			assetsStorage.registerAssetClass<core::AssetText>();
			assetsStorage.registerAssetClass<gameplay::AssetTexture>();
			assetsStorage.registerAssetClass<gameplay::AssetShader>();
			assetsStorage.registerAssetClass<gameplay::AssetSampler>();

			const bool useMultiThreading = false;
			const ThreadID renderingThread = useMultiThreading ? ThreadID::RenderingThread : ThreadID::Main;

			auto systemWindow = engineContext.addSystem<SystemWindow>("SystemWindow");
			auto systemImGui = engineContext.addSystem<SystemImGui>("SystemImGui", UpdatePhase::Main, renderingThread);
			auto systemRenderer = engineContext.addSystem<SystemRenderer>("SystemRenderer", UpdatePhase::Main, renderingThread);
			auto systemSave = engineContext.addSystem<SystemSave>("SystemSave");
			auto systemTickable = engineContext.addSystem<SystemTickable>("SystemTickable");
			auto systemThreadQueue = engineContext.addSystem<SystemThreadQueue>("SystemThreadQueue", UpdatePhase::Post);

			ASSERT_TRUE(engine.init());
			vulkan_renderer::ImGuiIntegration::SetStyle_Dark();

			Node2D::DefaultTexture = assetsStorage.getAs<AssetTexture>("Content/Textures/default_texture.png");
			Node2D::DefaultTexture->load(core::Paths::RootPath());

			auto& rootNode = engineContext.getRootNode();

			auto editorNode = CreateObject<NodeEditor>("Editor");
			rootNode->addChild(editorNode);
			systemImGui->addNode(editorNode);

			auto child = CreateObject<Node>("Child");
			rootNode->addChild(child);

			auto childOfChild = CreateObject<Node>("Child of child");
			child->addChild(childOfChild);

			{ // Sprite #1
				auto sprite = CreateObject<NodeSprite>("Sprite");
				sprite->transform.getPosition().x = -15;
				sprite->transform.getPosition().z = 50;
				sprite->transform.getScale().x = 24;
				sprite->transform.getScale().y = 24;
				sprite->texture = assetsStorage.getAs<AssetTexture>("Content/Textures/ships/ship_1.png");
				sprite->texture->load(core::Paths::RootPath());
				rootNode->addChild(sprite);
				systemTickable->addNode(sprite);
				systemRenderer->addNode(sprite);
			}

			{ // Sprite #2
				auto sprite = CreateObject<NodeSprite>("Sprite2");
				sprite->transform.getPosition().x = 15;
				sprite->transform.getPosition().z = 50;
				sprite->transform.getScale().x = 24;
				sprite->transform.getScale().y = 24;
				sprite->texture = assetsStorage.getAs<AssetTexture>("Content/Textures/ships/ship_0.png");
				sprite->texture->load(core::Paths::RootPath());
				rootNode->addChild(sprite);
				systemTickable->addNode(sprite);
				systemRenderer->addNode(sprite);
			}

			auto shaderVert = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
			ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
			systemRenderer->vertexShader = shaderVert;

			auto shaderFrag = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.frag");
			ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
			systemRenderer->fragmentShader = shaderFrag;

			auto nodeCamera = CreateObject<NodeCamera>("Camera");
			rootNode->addChild(nodeCamera);
			auto& camera = nodeCamera->getCamera();
			camera.setPosition(Vector3f(0.00001, 0, 150));
			camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			auto& window = systemWindow->getWindow();
			auto windowSize = window.getSize();
			camera.setFieldOfView(45.f);
			camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
			camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });

			systemRenderer->setCameraNode(nodeCamera);

			engine.loop();

			engine.deinit();
		}


	};
}