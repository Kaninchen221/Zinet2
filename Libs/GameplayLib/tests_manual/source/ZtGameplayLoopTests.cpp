#pragma once

#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"
#include "Zinet/GameplayLib/ZtSprite.hpp"

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
		const float flipbookFrameTime = 1000.f; // ms

		sf::RenderTarget spriteTexture;
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
				.time = flipbookFrameTime
			};

			FlipbookFrame frame1 = frame0;
			frame1.textureRegion = { { 0.25f, 0.f }, { 0.25f, 1.f } };

			FlipbookFrame frame2 = frame0;
			frame2.textureRegion = { { 0.50f, 0.f }, { 0.25f, 1.f } };

			flipbook->addFrame(frame1);
			flipbook->addFrame(frame2);
			flipbook->addFrame(frame1);
			flipbook->addFrame(frame0);
		}

		auto sprite = std::make_shared<Sprite>();
		{
			const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "cloud.png";
			if (!spriteTexture.loadFromFilePNG(path))
				FAIL() << "Can't load texture from file";

			sprite->setTexture(spriteTexture);
			sprite->setTextureRegion(RectF{ { 0.f, 0.f }, { 1.f, 1.f } });
		}

		gameplayLoop.addTickable(flipbook);
		gameplayLoop.addDrawable(flipbook);
		gameplayLoop.addDrawable(sprite);
		gameplayLoop.start();
	}
}