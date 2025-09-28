#pragma once

#include "Zinet/Core/ZtRandom.hpp"
#include "Zinet/Core/ZtTimeLog.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Gameplay/ZtEngine.hpp"

#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemThreadQueue.hpp"

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
			ZT_TIME_LOG(init());
		}

		void init()
		{
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

			auto& rootNode = engineContext.getRootNode();

			Node2D::DefaultTexture = assetsStorage.getAs<AssetTexture>("Content/Textures/default_texture.png");
			Node2D::DefaultTexture->load(core::Paths::RootPath());

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
			systemTickable->addNode(nodeCamera);

			auto editorNode = CreateObject<NodeEditor>("Editor");
			rootNode->addChild(editorNode);
			systemImGui->addNode(editorNode);

			auto child = CreateObject<Node>("Child");
			rootNode->addChild(child);

			auto childOfChild = CreateObject<Node>("Child of child");
			child->addChild(childOfChild);

			core::Random random;

			auto textureForSprites = assetsStorage.getAs<AssetTexture>("Content/Textures/image.png");

			ZT_TIME_LOG(
				ASSERT_TRUE(textureForSprites->load(core::Paths::RootPath()))
			);

			assert(false); // Disabled test

			ZT_TIME_LOG_NAME(CreateSprites,
				//for (size_t i = 0; i < 5; ++i)
				//{
				//	auto sprite = CreateObject<NodeSprite>(fmt::format("Sprite_{}", i));
				//	sprite->transform.getPosition().x = random.real<float>(-10, 10);
				//	sprite->transform.getPosition().y = random.real<float>(-10, 10);
				//	sprite->transform.getPosition().z = 50;
				//	sprite->transform.getScale().x = 8;
				//	sprite->transform.getScale().y = 8;
				//	sprite->transform.getRotation() = random.real<float>(0, 360);
				//	sprite->texture = textureForSprites;
				//	rootNode->addChild(sprite);
				//	//sprite->update(1.f);
				//	systemTickable->addNode(sprite);
				//	systemRenderer->addNode(sprite);
				//}
			);

			auto shaderVert = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
			ZT_TIME_LOG(
				ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
			);
			systemRenderer->vertexShader = shaderVert;

			auto shaderFrag = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.frag");
			ZT_TIME_LOG( // TODO: Optimize shader loading
				ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
			);
			systemRenderer->fragmentShader = shaderFrag;
		}

		void TearDown() override
		{
			ZT_TIME_LOG(
				engine.deinit();
			);
		}

		Engine engine;
	};

	TEST_F(EngineTests, PassTest)
	{
		engine.loop();
	}
}