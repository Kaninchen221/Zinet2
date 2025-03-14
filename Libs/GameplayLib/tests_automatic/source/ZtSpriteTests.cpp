#pragma once

#include "Zinet/GameplayLib/ZtSprite.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class SpriteTests : public ::testing::Test
	{
	protected:

		SpriteTests()
		{
		}

		~SpriteTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		Sprite sprite;
	};

	TEST_F(SpriteTests, GetDrawInfoTest)
	{
		sf::DrawInfo drawInfo = sprite.getDrawInfo();
	}
}