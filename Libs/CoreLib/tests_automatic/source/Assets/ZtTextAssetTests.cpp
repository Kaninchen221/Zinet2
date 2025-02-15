#pragma once

#include "Zinet/Core/Assets/ZtTextAsset.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class TextAssetTests : public ::testing::Test
	{
	protected:

		TextAssetTests()
		{
		}

		~TextAssetTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		TextAsset textAsset;
	};

	TEST_F(TextAssetTests, CanBeCreatedFromAssetTest)
	{
		ASSERT_TRUE(textAsset.canBeCreatedFromAsset());
	}
}