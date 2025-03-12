#pragma once

#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"

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
	};

	TEST_F(GameplayLoopTests, Test)
	{
		auto flipbook = std::make_shared<Flipbook>();
		{
			flipbookTexture.createEmpty({ 128, 128 }, sf::ColorFormat::R8G8B8A8_SRGB);
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

		gameplayLoop.addTickable(flipbook);
		gameplayLoop.addDrawable(flipbook);
		gameplayLoop.start();
	}
}