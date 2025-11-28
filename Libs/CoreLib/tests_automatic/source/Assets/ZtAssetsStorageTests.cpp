#pragma once

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class AssetsStorageTests : public ::testing::Test
	{
	protected:

		AssetsStorageTests()
		{
		}

		~AssetsStorageTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		AssetsStorage assetsStorage;
	};

	TEST_F(AssetsStorageTests, StoreAssetsTest)
	{
		assetsStorage.registerAssetClass<asset::Text>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto validAsset = assetsStorage.get("Content/placeholder.txt");
		ASSERT_TRUE(validAsset);

		auto invalidAsset = assetsStorage.get("invalid path");
		ASSERT_FALSE(invalidAsset);

		assetsStorage.unloadAssets();
		for (auto& asset : assetsStorage.getAssets())
		{
			EXPECT_FALSE(asset);
		}
	}

	TEST_F(AssetsStorageTests, GetAsTest)
	{
		assetsStorage.registerAssetClass<asset::Text>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetsStorage.getAs<asset::Text>("Content/placeholder.txt");
		ASSERT_TRUE(asset);

		const auto& constAssetsStorage = assetsStorage;

		auto constAsset = constAssetsStorage.getAs<asset::Text>("Content/placeholder.txt");
		ASSERT_TRUE(constAsset);
	}

	TEST_F(AssetsStorageTests, LoadMinimalAssetTest)
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

		auto optionalAsset = assetsStorage.loadAssetMetaData(findAssetsResult.assets.front());

		ASSERT_TRUE(optionalAsset);

		auto asset = std::move(optionalAsset.value());
		ASSERT_TRUE(!asset.getMetaData().empty());

		auto extension = asset.getMetaData().value("fileExtension", "");
		ASSERT_FALSE(extension.empty());

		auto Logger = ConsoleLogger::Create("temp");
		Logger->info("Asset extension: {}", extension);
	}
}