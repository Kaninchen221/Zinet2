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
		}

		void TearDown() override {
		}

		TileMap tileMap;
	};

	TEST_F(TileMapTests, GetDrawInfoTest)
	{
		sf::DrawInfo drawInfo = tileMap.getDrawInfo();
	}
}