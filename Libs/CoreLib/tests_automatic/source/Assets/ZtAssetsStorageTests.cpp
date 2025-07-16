#pragma once

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
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

	struct TextAsset : public Asset
	{
		TextAsset() : Asset{ "txt" } {}

		AssetHandle<Asset> createCopy() const ZINET_API_POST override { return std::make_shared<TextAsset>(*this); }

		bool load(const Path& rootPath) ZINET_API_POST override
		{
			File file;
			const auto filePath = rootPath / metaData.value("fileRelativePath", "");
			file.open(filePath, FileOpenMode::Read);
			if (!file.isOpen())
			{
				Logger->error("Couldn't open file, path: {}", filePath.generic_string());
				return {};
			}
			
			auto rawData = file.readData();
			file.close();

			text = std::string{ rawData.begin(), rawData.end() };
			return true;
		}

		void unload() ZINET_API_POST override { text.clear(); }

		// Content
		std::string text;

	};

	TEST_F(AssetsStorageTests, StoreAssetsTest)
	{
		assetsStorage.registerAssetClass<TextAsset>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto validAsset = assetsStorage.get("Content/placeholder.txt");
		ASSERT_TRUE(validAsset);

		auto invalidAsset = assetsStorage.get("invalid path");
		ASSERT_FALSE(invalidAsset);
	}

	TEST_F(AssetsStorageTests, LoadMinimalAssetTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = false,
			.reimport = false
		};

		AssetsFinder assetsFinder;
		assetsFinder.rootFolder = Paths::CurrentProjectRootPath() / "assets_finder_test_folder";
		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinder.findAssets(findAssetsInput);

		EXPECT_EQ(findAssetsResult.files.size(), 1u);
		EXPECT_EQ(findAssetsResult.assets.size(), 1u);

		auto optionalAsset = assetsStorage.loadAssetMetaData(findAssetsResult.assets.front());

		ASSERT_TRUE(optionalAsset);

		auto asset = std::move(optionalAsset.value());
		ASSERT_TRUE(!asset.metaData.empty());

		auto extension = asset.metaData.value("fileExtension", "");
		ASSERT_FALSE(extension.empty());

		auto Logger = ConsoleLogger::Create("temp");
		Logger->info("Asset extension: {}", extension);
	}

	TEST_F(AssetsStorageTests, TextAssetTest)
	{
		assetsStorage.registerAssetClass<TextAsset>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto textAsset = assetsStorage.getAs<TextAsset>("Content/placeholder.txt");
		ASSERT_TRUE(textAsset);

		ASSERT_TRUE(textAsset->load(assetsStorage.assetsFinder.rootFolder));
		ASSERT_FALSE(textAsset->text.empty());

		textAsset->unload();
		ASSERT_TRUE(textAsset->text.empty());
	}
}