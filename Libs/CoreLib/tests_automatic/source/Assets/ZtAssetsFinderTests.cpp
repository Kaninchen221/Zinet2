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
			assetsFinder.setRootFolder(Paths::CurrentProjectRootPath() / "assets_finder_test_folder");
		}

		void TearDown() override
		{
		}

		AssetsFinder assetsFinder;
		const AssetsFinder& assetsFinderConst = assetsFinder;
	};

	TEST(AssetsFinderSimpleTests, GetContentFolderNameTest)
	{
		const AssetsFinder assetsFinder;
		const std::string& contentFolderName = assetsFinder.getContentFolderName();
		EXPECT_EQ(contentFolderName, "Content");
	}

	TEST(AssetsFinderSimpleTests, GetContentFolderPathTest)
	{
		const AssetsFinder assetsFinder;
		auto contentFolderPath = assetsFinder.getContentFolderPath();

		EXPECT_EQ(contentFolderPath, Paths::RootPath() / assetsFinder.getContentFolderName());
	}

	TEST(AssetsFinderSimpleTests, ContentFolderExistsTest)
	{
		const AssetsFinder assetsFinder;
		EXPECT_TRUE(assetsFinder.contentFolderExists());
	}

	TEST(AssetsFinderSimpleTests, GetAssetExtensionTest)
	{
		const AssetsFinder assetsFinder;
		EXPECT_EQ(assetsFinder.getAssetFileExtension(), "asset");
	}

	TEST_F(AssetsFinderTests, FindAssetsTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = true,
			.reimport = true
		};

		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinderConst.findAssets(findAssetsInput);

		EXPECT_EQ(findAssetsResult.filesPaths.size(), 3u);
		EXPECT_EQ(findAssetsResult.assetsFilesPaths.size(), 3u);
	}

	TEST_F(AssetsFinderTests, IsAssetFilePathTest)
	{
		const std::filesystem::path assetPath = "shader.vert.asset";
		EXPECT_TRUE(assetsFinder.isAssetFilePath(assetPath));

		const std::filesystem::path notAssetPath = "shader.vert";
		EXPECT_FALSE(assetsFinder.isAssetFilePath(notAssetPath));

		const std::filesystem::path directoryPath = "test/folder";
		EXPECT_FALSE(assetsFinder.isAssetFilePath(directoryPath));
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

	TEST_F(AssetsFinderTests, CreateAssetsObjectsTest)
	{
		AssetsFinder::FindAssetsInput findAssetsInput
		{
			.recursive = true,
			.reimport = false
		};

		const AssetsFinder::FindAssetsResult findAssetsResult = assetsFinderConst.findAssets(findAssetsInput);

		const AssetsFinder::CreateAssetsResult createAssetsResult = assetsFinderConst.createAssets(findAssetsResult);

		EXPECT_EQ(createAssetsResult.assets.size(), 3u);

		for (const auto& asset : createAssetsResult.assets)
		{
			EXPECT_FALSE(asset.getMetaData().is_null());
		}
	}
}