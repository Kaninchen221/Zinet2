#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemFlipBooks.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedFlipBook.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"
#include "Zinet/Core/ZtRandom.hpp"

#include <gtest/gtest.h>

#include <thread>

namespace zt::gameplay::tests
{
	class SystemFlipBooksTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			//SystemRenderer::SetUseImGui(false);

			auto& assetsStorage = engineContext.getAssetsStorage();
			assetsStorage.registerAssetClass<gameplay::AssetShader>();
			assetsStorage.registerAssetClass<gameplay::AssetSampler>();
			assetsStorage.registerAssetClass<gameplay::AssetTexture>();

			systemWindow = engineContext.addSystem<SystemWindow>("window");
			systemImGui = engineContext.addSystem<SystemImGui>("imgui");
			systemRenderer = engineContext.addSystem<SystemRenderer>("renderer");
			systemTickable = engineContext.addSystem<SystemTickable>("tickable");
			system = engineContext.addSystem<SystemFlipBooks>("sprites", UpdatePhase::Pre);

			ASSERT_TRUE(engineContext.init());

			auto nodeEditor = CreateObject<NodeEditor>("Editor");
			engineContext.getRootNode()->addChild(nodeEditor);
			systemImGui->addNode(nodeEditor);

			auto nodeCamera = CreateObject<NodeCamera>("Camera");
			auto& camera = nodeCamera->getCamera();
			camera.setPosition(Vector3f(100, 100, 1000));
			camera.setLookingAt(Vector3f(100, 100, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			auto& window = systemWindow->getWindow();
			auto windowSize = window.getSize();
			camera.setFieldOfView(45.f);
			camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
			camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });

			systemRenderer->setCameraNode(nodeCamera);
			systemTickable->addNode(nodeCamera);
			engineContext.getRootNode()->addChild(nodeCamera);

			auto texture = assetsStorage.getAs<AssetTexture>("Content/Textures/player.png");
			ASSERT_TRUE(texture->load(core::Paths::RootPath()));
			system->setAssetTexture(texture);

			auto shaderVert = assetsStorage.getAs<AssetShader>("Content/Shaders/shader_flip_books.vert");
			ASSERT_TRUE(shaderVert);
			ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
			systemRenderer->vertexShader = shaderVert;

			auto shaderFrag = assetsStorage.getAs<AssetShader>("Content/Shaders/shader_flip_books.frag");
			ASSERT_TRUE(shaderFrag);
			ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
			systemRenderer->fragmentShader = shaderFrag;
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
		ObjectHandle<SystemTickable> systemTickable;
		ObjectHandle<SystemWindow> systemWindow;
		ObjectHandle<SystemRenderer> systemRenderer;
		ObjectHandle<SystemImGui> systemImGui;

		using SystemT = SystemFlipBooks;
		ObjectHandle<SystemT> system;

		using NodeT = NodeInstancedFlipBook;
		std::vector<ObjectHandle<NodeT>> nodes;

		ObjectHandle<NodeT> AddNodeTest(
			[[maybe_unused]] const auto x,
			[[maybe_unused]] const auto y, 
			[[maybe_unused]] const auto width, 
			[[maybe_unused]] const auto height)
		{
			auto& transforms = system->getTransforms();
		
			ObjectHandle<NodeT> node = CreateObject<NodeT>("NodeName");
			nodes.push_back(node);

			system->addNode(node);
			EXPECT_EQ(node->getID(), nodes.size() - 1);
			EXPECT_EQ(&node->getTransform(), &transforms[node->getID()]);
		
			EXPECT_EQ(transforms.size(), nodes.size());

			const Vector3f position =
			{
				x,
				y,
				50
			};

			const float scale = 1;

			const float rotation = 0;

			auto& transform = node->getTransform();
			transform.setScale(Vector3f(scale, scale, 1.0f));
			transform.setPosition(position);
			transform.setRotation(rotation);

			auto& frames = node->getFrames();
			FlipBookFrame frame
			{
				.textureCoords =
				{
					Vector2f{ 0, 0 },
					{ 0.33, 0 },
					{ 0.33, 0.20 },
					{ 0, 0.20 }
				},
				.time = 1.f
			};
			frames.push_back(frame);

			return node;
		}

		// TODO
		// A node class just to pass the descriptor info from the SystemFlipBooks to the renderer
		// Perhaps the SystemFlipBooks should communicate with the renderer system directly instead of using a fake node
		class NodeBridge : public Node2D
		{
		private:

		public:

			NodeBridge() = default;
			NodeBridge(const NodeBridge& other) = default;
			NodeBridge(NodeBridge&& other) noexcept = default;
			~NodeBridge() noexcept = default;

			NodeBridge& operator = (const NodeBridge& other) = default;
			NodeBridge& operator = (NodeBridge&& other) noexcept = default;

			vulkan_renderer::DescriptorInfo getDescriptorInfo() override
			{
				auto& localEngineContext = EngineContext::Get();
				auto localSystem = localEngineContext.getSystem<SystemT>();
				if (!Ensure(localSystem))
					return {};

				return localSystem->getDescriptorInfo();
			}

			uint32_t instancesCount = 1;
			uint32_t getInstancesCount() const noexcept override { return instancesCount; }

		};
	};

	TEST_F(SystemFlipBooksTests, PassTest)
	{
		const size_t width = 1;
		const size_t height = 1;
		for (size_t x = 0; x < width; ++x)
			for (size_t y = 0; y < height; ++y)
				AddNodeTest(x, y, width, height);

		auto& camera = systemRenderer->getCameraNode()->getCamera();
		camera.setPosition(Vector3f(width / 2.f, height / 2.f, 55));
		camera.setLookingAt(Vector3f(width / 2.f, height / 2.f, 0));

		system->update();

		auto nodeBridge = CreateObject<NodeBridge>("Fake Sprite");
		nodeBridge->instancesCount = static_cast<uint32_t>(nodes.size());
		engineContext.getRootNode()->addChild(nodeBridge);
		systemRenderer->addNode(nodeBridge);

		std::jthread exitThread(
		[&engineContext = engineContext]()
		{
			while (!engineContext.isLooping())
			{
			}
		
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
			engineContext.stopLooping();
		});

		engineContext.loop();
	}
}