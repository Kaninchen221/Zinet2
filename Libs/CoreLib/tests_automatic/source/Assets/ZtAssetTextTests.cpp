#pragma once

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class AssetTextTests : public ::testing::Test
	{
	protected:

		AssetTextTests()
		{
		}

		~AssetTextTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		AssetText assetText;
	};

	TEST_F(AssetTextTests, Test)
	{
		AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<AssetText>("AssetText");

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto textAsset = assetsStorage.getAs<AssetText>("Content/placeholder.txt");
		ASSERT_TRUE(textAsset);

		ASSERT_TRUE(textAsset->load(assetsStorage.assetsFinder.getRootPath()));
		ASSERT_TRUE(textAsset->isLoaded());
		ASSERT_FALSE(textAsset->text.empty());

		textAsset->unload();
		ASSERT_FALSE(textAsset->isLoaded());
		ASSERT_TRUE(textAsset->text.empty());
	}
}