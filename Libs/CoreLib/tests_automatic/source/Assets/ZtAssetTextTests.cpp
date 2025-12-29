#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

#include <gtest/gtest.h>

namespace zt::core::asset::tests
{
	class TextTests : public ::testing::Test
	{
	protected:

		Text assetText;
	};

	TEST_F(TextTests, Test)
	{
		AssetStorage assetStorage;
		assetStorage.registerAssetClass<Text>();

		bool result = assetStorage.storeAssets();
		ASSERT_TRUE(result);

		auto textAsset = assetStorage.getAs<Text>("Content/placeholder.txt");
		ASSERT_TRUE(textAsset);

		ASSERT_TRUE(textAsset->load());
		ASSERT_TRUE(textAsset->isLoaded());
		ASSERT_FALSE(textAsset->getText().empty());

		textAsset->unload();
		ASSERT_FALSE(textAsset->isLoaded());
		ASSERT_TRUE(textAsset->getText().empty());
	}
}