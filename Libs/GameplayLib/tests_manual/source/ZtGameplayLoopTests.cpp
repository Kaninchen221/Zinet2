#pragma once

#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"
#include "Zinet/GameplayLib/ZtSprite.hpp"
#include "Zinet/GameplayLib/ZtTileMap.hpp"

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

		GameplayLoop gameplayLoop;

		sf::RenderTarget flipbookTexture;
		const float flipbookFrameTimeMs = 1000.f;

		sf::RenderTarget spriteTexture;

		sf::RenderTarget tileSetTexture;
	};

	TEST_F(GameplayLoopTests, Test)
	{
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
			flipbook->setPosition(flipbook->getSize() / -2.f); // Center it, the pivot is in the upper left corner
		}

		auto sprite = std::make_shared<Sprite>();
		{
			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "cloud.png";
			if (!spriteTexture.loadFromFilePNG(path))
				FAIL() << "Can't load texture from file";

			sprite->setTexture(spriteTexture);
			sprite->setTextureRegion(RectF{ { 0.f, 0.f }, { 1.f, 1.f } });
			sprite->setSize({ 48, 48 });
			sprite->setPosition(sprite->getSize() / -2.f - Vector2f{ 0.f, 100.f });
		}

		auto tileMap = std::make_shared<TileMap>();
		{
			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "tile_set.png";
			if (!tileSetTexture.loadFromFilePNG(path))
				FAIL() << "Can't load texture from file";

			tileMap->setTexture(tileSetTexture);
			tileMap->setTileSizeInTexture(Vector2ui{ 16u, 16u });
			tileMap->setTilesCount(Vector2ui{ 3u, 3u });
			tileMap->setTiles(
			{
				{ 0, 0 }, { 1, 0 }, { 2, 0 },
				{ 0, 1 }, { 1, 1 }, { 2, 1 },
				{ 0, 2 }, { 1, 2 }, { 2, 2 }
			});

			tileMap->setSize({ 128, 128 });
			tileMap->setPosition(tileMap->getSize() / -2.f); // Center it, the pivot is in the upper left corner
		}

		auto camera = std::make_shared<Camera>();
		{
			camera->create();
			//camera->setPosition({ -100, -100 });
		}
		gameplayLoop.setCurrentCamera(camera);

		gameplayLoop.addTickable(flipbook);

		gameplayLoop.addDrawable(tileMap);
		gameplayLoop.addDrawable(flipbook);
		gameplayLoop.addDrawable(sprite);

		gameplayLoop.start();
	}
}