#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedSprite.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

#include <thread>

namespace zt::gameplay::tests
{
	class SystemSpritesTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			SystemRenderer::SetUseImGui(false);

			auto& assetsStorage = engineContext.getAssetsStorage();
			assetsStorage.registerAssetClass<gameplay::AssetShader>();

			engineContext.addSystem<SystemWindow>("window");
			systemRenderer = engineContext.addSystem<SystemRenderer>("renderer");
			systemSprites = engineContext.addSystem<SystemSprites>("sprites", UpdatePhase::Pre);
			// TODO: Every object should have self handle automatically set in the ObjectsStorage class
			systemSprites->self = systemSprites;

			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
		ObjectHandle<SystemSprites> systemSprites;
		ObjectHandle<SystemRenderer> systemRenderer;
		std::vector<ObjectHandle<NodeInstancedSprite>> sprites;

		ObjectHandle<NodeInstancedSprite> AddSpriteTest()
		{
			auto& objectsStorage = engineContext.getObjectsStorage();
			auto& transforms = systemSprites->getTransforms();
		
			ObjectHandle<NodeInstancedSprite> sprite = objectsStorage.createObject<NodeInstancedSprite>("Sprite");
			sprites.push_back(sprite);

			systemSprites->addNode(sprite);
			EXPECT_EQ(sprite->getID(), sprites.size() - 1);
			EXPECT_EQ(&sprite->getTransform(), &transforms[sprite->getID()]);
		
			EXPECT_EQ(transforms.size(), sprites.size());
		
			return sprite;
		}

		// TODO
		// A fake sprite class just to pass the descriptor info from the SystemSprites to the renderer
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

			vulkan_renderer::DescriptorInfo getDrawCallDescriptorInfos() override
			{
				auto& engContext = EngineContext::Get();
				auto sysSprites = engContext.getSystem<SystemSprites>();
				if (!Ensure(sysSprites))
					return {};

				return sysSprites->getDescriptorInfo();
			}

		};
	};

	TEST_F(SystemSpritesTests, PassTest)
	{
		AddSpriteTest();
		AddSpriteTest();
		AddSpriteTest();

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

		auto& objectsStorage = engineContext.getObjectsStorage();
		auto fakeSprite = objectsStorage.createObject<FakeSprite>("Fake Sprite");
		engineContext.getRootNode()->addChild(fakeSprite);
		systemRenderer->addNode(fakeSprite);

		std::jthread exitThread(
		[&engineContext = engineContext]()
		{
			while (!engineContext.isLooping())
			{}

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
			engineContext.stopLooping();
		});

		engineContext.loop();
	}
}