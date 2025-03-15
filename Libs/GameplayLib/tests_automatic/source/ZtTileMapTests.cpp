#pragma once

#include "Zinet/GameplayLib/ZtTileMap.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class TileMapTests : public ::testing::Test
	{
	protected:

		TileMapTests()
		{
		}

		~TileMapTests() override {
		}

		void SetUp() override 
		{
			renderTarget.createEmpty({ 1, 1 }, software_renderer::ColorFormat::R8G8B8A8_SRGB);
			tileMap.setTexture(renderTarget);
		}

		void TearDown() override {
		}

		TileMap tileMap;
		sf::RenderTarget renderTarget;
	};

	TEST_F(TileMapTests, GetDrawInfoTest)
	{
		sf::DrawInfo drawInfo = tileMap.getDrawInfo();
	}
}