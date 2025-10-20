#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

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
		AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<Text>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto textAsset = assetsStorage.getAs<Text>("Content/placeholder.txt");
		ASSERT_TRUE(textAsset);

		ASSERT_TRUE(textAsset->load(Paths::RootPath()));
		ASSERT_TRUE(textAsset->isLoaded());
		ASSERT_FALSE(textAsset->getText().empty());

		textAsset->unload();
		ASSERT_FALSE(textAsset->isLoaded());
		ASSERT_TRUE(textAsset->getText().empty());
	}
}