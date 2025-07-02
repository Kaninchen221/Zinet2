#pragma once

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class AssetsFinderTests : public ::testing::Test
	{
	protected:

		AssetsFinderTests()
		{
		}

		~AssetsFinderTests() override
		{
		}

		void SetUp() override
		{
			assetsFinder.rootFolder = Paths::CurrentProjectRootPath() / "assets_finder_test_folder";
		}

		void TearDown() override
		{
		}

		AssetsFinder assetsFinder;
		const AssetsFinder& assetsFinderConst = assetsFinder;
	};

	TEST_F(AssetsFinderTests, FindAssetsTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = true,
			.reimport = true
		};

		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinderConst.findAssets(findAssetsInput);

		EXPECT_EQ(findAssetsResult.files.size(), 3u);
		EXPECT_EQ(findAssetsResult.assets.size(), 3u);
	}

	TEST_F(AssetsFinderTests, IsAssetFilePathTest)
	{
		const std::filesystem::path assetPath = "shader.vert.asset";
		EXPECT_TRUE(assetsFinder.isAssetFile(assetPath));

		const std::filesystem::path notAssetPath = "shader.vert";
		EXPECT_FALSE(assetsFinder.isAssetFile(notAssetPath));

		const std::filesystem::path directoryPath = "test/folder";
		EXPECT_FALSE(assetsFinder.isAssetFile(directoryPath));
	}

	TEST_F(AssetsFinderTests, CreateAssetFilePathTest)
	{
		const std::filesystem::path filePath = "TestFolder/Content/Folder2/textFile.txt";
		const std::filesystem::path expectedPath = "TestFolder/Content/Folder2/textFile.txt.asset";
		const std::filesystem::path actualPath = assetsFinder.createAssetFilePath(filePath);

		EXPECT_EQ(expectedPath, actualPath);
	}

	TEST_F(AssetsFinderTests, CreateRelativePathTest)
	{
		const std::filesystem::path filePath = "C:/TestFolder/Content/Folder2/textFile.txt";
		const std::filesystem::path expectedPath = "Content/Folder2/textFile.txt";
		const std::filesystem::path actualPath = assetsFinder.createRelativePath("Content", filePath);

		EXPECT_EQ(expectedPath, actualPath);
	}

	TEST_F(AssetsFinderTests, LoadAssetTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = false,
			.reimport = false
		};

		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinderConst.findAssets(findAssetsInput);

		EXPECT_EQ(findAssetsResult.files.size(), 1u);
		EXPECT_EQ(findAssetsResult.assets.size(), 1u);

		Asset asset = assetsFinder.loadAsset(findAssetsResult.files.front(), findAssetsResult.assets.front());

		ASSERT_TRUE(!asset.rawData.empty());
		ASSERT_TRUE(!asset.metaData.empty());

		auto extension = asset.metaData["fileExtension"];
		ASSERT_TRUE(extension.is_string());

		auto Logger = core::ConsoleLogger::Create("temp");
		Logger->info("Asset extension: {}", extension.get<std::string_view>());
		Logger->info(std::string_view(reinterpret_cast<char*>(asset.rawData.data()), asset.rawData.size()));
	}
}