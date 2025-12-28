#pragma once

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class AssetStorageTests : public ::testing::Test
	{
	protected:

		AssetStorage assetStorage;
		inline static std::string AssetKey = "Content/placeholder.txt";
		inline static std::string InvalidAssetKey = "invalid path";
	};

	TEST_F(AssetStorageTests, StoreAssetsTest)
	{
		assetStorage.registerAssetClass<asset::Text>();

		bool result = assetStorage.storeAssets();
		ASSERT_TRUE(result);

		AssetHandle validAsset = assetStorage.get(AssetKey);
		ASSERT_TRUE(validAsset);

		AssetHandle invalidAsset = assetStorage.get(InvalidAssetKey);
		ASSERT_FALSE(invalidAsset);

		assetStorage.unloadAssets();
		for (auto& asset : assetStorage.getAssets())
		{
			EXPECT_FALSE(asset);
		}
	}

	TEST_F(AssetStorageTests, GetTest)
	{
		assetStorage.registerAssetClass<asset::Text>();

		bool result = assetStorage.storeAssets();
		ASSERT_TRUE(result);
		
		AssetHandle asset = assetStorage.get(AssetKey);
		ASSERT_TRUE(asset);

		const auto& constAssetStorage = assetStorage;

		ConstAssetHandle constAsset = constAssetStorage.get(AssetKey);
		ASSERT_TRUE(constAsset);
	}

	TEST_F(AssetStorageTests, GetAsTest)
	{
		assetStorage.registerAssetClass<asset::Text>();

		bool result = assetStorage.storeAssets();
		ASSERT_TRUE(result);

		AssetHandle asset = assetStorage.getAs<asset::Text>(AssetKey);
		ASSERT_TRUE(asset);

		const auto& constAssetStorage = assetStorage;

		ConstAssetHandle constAsset = constAssetStorage.getAs<asset::Text>(AssetKey);
		ASSERT_TRUE(constAsset);
	}

	TEST_F(AssetStorageTests, LoadMinimalAssetTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = false,
			.reimport = false
		};

		AssetsFinder assetsFinder;
		assetsFinder.setRootPath(Paths::CurrentProjectRootPath() / "assets_finder_test_folder");
		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinder.findAssets(findAssetsInput);

		EXPECT_EQ(findAssetsResult.files.size(), 1u);
		EXPECT_EQ(findAssetsResult.assets.size(), 1u);

		auto optionalAsset = assetStorage.loadAssetMetaData(findAssetsResult.assets.front());

		ASSERT_TRUE(optionalAsset);

		auto asset = std::move(optionalAsset.value());
		ASSERT_TRUE(!asset.getMetaData().empty());

		auto extension = asset.getMetaData().value("fileExtension", "");
		ASSERT_FALSE(extension.empty());

		auto Logger = ConsoleLogger::Create("temp");
		Logger->info("Asset extension: {}", extension);
	}
}