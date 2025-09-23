#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedSprite.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"
#include "Zinet/Core/ZtRandom.hpp"

#include <gtest/gtest.h>

#include <thread>

namespace zt::gameplay::tests
{
	class SystemSpritesTests : public ::testing::Test
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
			systemSprites = engineContext.addSystem<SystemSprites>("sprites", UpdatePhase::Pre);

			ASSERT_TRUE(engineContext.init());

			auto nodeEditor = CreateObject<NodeEditor>("Editor");
			engineContext.getRootNode()->addChild(nodeEditor);
			systemImGui->addNode(nodeEditor);

			auto nodeCamera = CreateObject<NodeCamera>("Camera");
			auto& camera = nodeCamera->getCamera();
			camera.setPosition(Vector3f(0.00001, 0, 10000));
			camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			auto& window = systemWindow->getWindow();
			auto windowSize = window.getSize();
			camera.setFieldOfView(45.f);
			camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
			camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });

			systemRenderer->setCameraNode(nodeCamera);
			systemTickable->addNode(nodeCamera);
			engineContext.getRootNode()->addChild(nodeCamera);

			auto texture = engineContext.getAssetsStorage().getAs<AssetTexture>("Content/Textures/image.png");
			ASSERT_TRUE(texture->load(core::Paths::RootPath()));
			systemSprites->setAssetTexture(texture);
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
		ObjectHandle<SystemTickable> systemTickable;
		ObjectHandle<SystemWindow> systemWindow;
		ObjectHandle<SystemSprites> systemSprites;
		ObjectHandle<SystemRenderer> systemRenderer;
		ObjectHandle<SystemImGui> systemImGui;
		std::vector<ObjectHandle<NodeInstancedSprite>> sprites;

		ObjectHandle<NodeInstancedSprite> AddSpriteTest(
			[[maybe_unused]] const auto x,
			[[maybe_unused]] const auto y, 
			[[maybe_unused]] const auto width, 
			[[maybe_unused]] const auto height)
		{
			auto& transforms = systemSprites->getTransforms();
		
			ObjectHandle<NodeInstancedSprite> sprite = CreateObject<NodeInstancedSprite>("Sprite");
			sprites.push_back(sprite);

			systemSprites->addNode(sprite);
			EXPECT_EQ(sprite->getID(), sprites.size() - 1);
			EXPECT_EQ(&sprite->getTransform(), &transforms[sprite->getID()]);
		
			EXPECT_EQ(transforms.size(), sprites.size());

			core::Random random;

			const Vector3f position =
			{
				random.real<float>(-1000, 1000),
				random.real<float>(-1000, 1000),
				50.f
			};

			const float scale = random.real<float>(5, 15);

			const float rotation = random.real<float>(0, 360);

			auto& transform = sprite->getTransform();
			transform.setScale(Vector3f(scale, scale, 1.0f));
			transform.setPosition(position);
			transform.setRotation(rotation);

			return sprite;
		}

		// TODO
		// A fake sprite class just to pass the descriptor info from the SystemSprites to the renderer
		// Perhaps the SystemSprites should communicate with the renderer system directly instead of using a fake node
		class FakeSprite : public Node2D
		{
		private:

		public:

			FakeSprite() = default;
			FakeSprite(const FakeSprite& other) = default;
			FakeSprite(FakeSprite&& other) noexcept = default;
			~FakeSprite() noexcept = default;

			FakeSprite& operator = (const FakeSprite& other) = default;
			FakeSprite& operator = (FakeSprite&& other) noexcept = default;

			vulkan_renderer::DescriptorInfo getDescriptorInfo() override
			{
				auto& engContext = EngineContext::Get();
				auto sysSprites = engContext.getSystem<SystemSprites>();
				if (!Ensure(sysSprites))
					return {};

				return sysSprites->getDescriptorInfo();
			}

			uint32_t instancesCount = 1;
			uint32_t getInstancesCount() const noexcept override { return instancesCount; }

		};
	};

	TEST_F(SystemSpritesTests, PassTest)
	{
		const size_t width = 100;
		const size_t height = 100;
		for (size_t x = 0; x < width; ++x)
			for (size_t y = 0; y < height; ++y)
				AddSpriteTest(x, y, width, height);

		systemSprites->update();

		auto descriptorInfo = systemSprites->getDescriptorInfo();
		ASSERT_TRUE(descriptorInfo.buffersPerType.size() == 1);
		ASSERT_TRUE(descriptorInfo.buffersPerType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].size() == 1);
		ASSERT_TRUE(descriptorInfo.buffersPerType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].front());

		auto& assetsStorage = engineContext.getAssetsStorage();

		auto shaderVert = assetsStorage.getAs<AssetShader>("Content/Shaders/shader_sprites.vert");
		ASSERT_TRUE(shaderVert);
		ASSERT_TRUE(shaderVert->load(core::Paths::RootPath()));
		systemRenderer->vertexShader = shaderVert;

		auto shaderFrag = assetsStorage.getAs<AssetShader>("Content/Shaders/shader_sprites.frag");
		ASSERT_TRUE(shaderFrag);
		ASSERT_TRUE(shaderFrag->load(core::Paths::RootPath()));
		systemRenderer->fragmentShader = shaderFrag;

		auto fakeSprite = CreateObject<FakeSprite>("Fake Sprite");
		fakeSprite->instancesCount = static_cast<uint32_t>(sprites.size());
		engineContext.getRootNode()->addChild(fakeSprite);
		systemRenderer->addNode(fakeSprite);

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