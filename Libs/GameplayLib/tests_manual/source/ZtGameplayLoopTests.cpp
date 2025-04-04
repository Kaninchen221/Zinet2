#pragma once

#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"
#include "Zinet/GameplayLib/ZtSprite.hpp"
#include "Zinet/GameplayLib/ZtTileMap.hpp"
#include "Zinet/GameplayLib/ZtHorizontalSlidingWorld.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay_lib::tests
{

	class GameplayLoopTests : public ::testing::Test
	{
	protected:

		GameplayLoopTests()
		{
		}

		~GameplayLoopTests() override {
		}

		void SetUp() override {
		}

		void TearDown() override {
		}

		inline static auto Logger = core::ConsoleLogger::Create("GameplayLoopTests");

		GameplayLoop gameplayLoop;

		sf::RenderTarget flipbookTexture;
		const float flipbookFrameTimeMs = 1000.f;

		sf::RenderTarget spriteTexture;
		sf::RenderTarget dragSpriteTexture;
		sf::RenderTarget closeIconTexture;
		sf::RenderTarget tileSetTexture;
	};

	TEST_F(GameplayLoopTests, Test)
	{
		try
		{
			auto camera = std::make_shared<Camera>();
			{
				camera->create();
				camera->setLookAt({ 0, 0 });
			}
			gameplayLoop.setCurrentCamera(camera);

			auto dragSprite = std::make_shared<Sprite>();
			{
				const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "drag_icon.png";
				if (!dragSpriteTexture.loadFromFilePNG(path))
					FAIL() << "Can't load texture from file";

				dragSprite->setTexture(dragSpriteTexture);
				dragSprite->setTextureRegion(RectF{ { 0.f, 0.f }, { 1.f, 1.f } });
				dragSprite->setSize({ 16, 16 });
				dragSprite->setPosition(dragSprite->getSize() / -2.f - Vector2f{ 200.f, 200.f });
			}

			auto horizontalSlidingWorld = std::make_shared<HorizontalSlidingWorld>();
			{
				horizontalSlidingWorld->init(core::Paths::CurrentProjectRootPath() / "test_files", horizontalSlidingWorld);
				horizontalSlidingWorld->setParentNode(dragSprite);
				horizontalSlidingWorld->setPosition(Vector2f{ -50.f, 100.f });
				horizontalSlidingWorld->setDeadEndX(camera->getSize().x / -2.f - horizontalSlidingWorld->getSliceSize().x);
			}

			auto closeIcon = std::make_shared<Sprite>();
			{
				const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "close_icon.png";
				if (!closeIconTexture.loadFromFilePNG(path))
					FAIL() << "Can't load texture from file";

				closeIcon->setTexture(closeIconTexture);
				closeIcon->setTextureRegion(RectF{ { 0.f, 0.f }, { 1.f, 1.f } });
				closeIcon->setSize({ 16, 16 });
				closeIcon->setPosition(Vector2f{ 300.f, 0.f });
				closeIcon->setParentNode(dragSprite);
			}

			auto flipbook = std::make_shared<Flipbook>();
			{
				const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "player.png";
				if (!flipbookTexture.loadFromFilePNG(path))
					FAIL() << "Can't load texture from file";

				FlipbookFrame frame0
				{
					.texture = flipbookTexture,
					.textureRegion = { { 0.f, 0.f }, { 0.25f, 1.f } },
					.time = flipbookFrameTimeMs
				};

				FlipbookFrame frame1 = frame0;
				frame1.textureRegion = { { 0.25f, 0.f }, { 0.25f, 1.f } };

				FlipbookFrame frame2 = frame0;
				frame2.textureRegion = { { 0.50f, 0.f }, { 0.25f, 1.f } };

				flipbook->addFrame(frame1);
				flipbook->addFrame(frame2);
				flipbook->addFrame(frame1);
				flipbook->addFrame(frame0);
				flipbook->setSize({ 32, 32 });
				flipbook->setParentNode(dragSprite);
				flipbook->setPosition(Vector2f{ 150.f, 150.f });
			}

			auto sprite = std::make_shared<Sprite>();
			{
				const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "cloud.png";
				if (!spriteTexture.loadFromFilePNG(path))
					FAIL() << "Can't load texture from file";

				sprite->setTexture(spriteTexture);
				sprite->setTextureRegion(RectF{ { 0.f, 0.f }, { 1.f, 1.f } });
				sprite->setSize({ 48, 48 });
				sprite->setPosition(-Vector2f{ 0.f, 100.f });
				sprite->setParentNode(flipbook);
			}
			
			gameplayLoop.tickableSystem.addNode(flipbook);

			gameplayLoop.drawableSystem.addNode(sprite);
			gameplayLoop.drawableSystem.addNode(dragSprite);
			gameplayLoop.drawableSystem.addNode(closeIcon);
			gameplayLoop.drawableSystem.addNode(flipbook);

			gameplayLoop.tickableSystem.addNode(horizontalSlidingWorld);

			gameplayLoop.windowEventsSystem.addDragableNode(sprite);
			gameplayLoop.windowEventsSystem.addDragableNode(dragSprite);

			wd::Window& window = gameplayLoop.window;
			gameplayLoop.windowEventsSystem.addClickableNode(closeIcon, [&window](const std::weak_ptr<Node>& node) { window.requestCloseWindow(); });

			gameplayLoop.start();
		}
		catch (const std::exception& ex)
		{
			FAIL() << ex.what();
		}
	}
}