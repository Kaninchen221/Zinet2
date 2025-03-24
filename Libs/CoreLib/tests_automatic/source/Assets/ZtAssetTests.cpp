#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/Assets/ZtTextAsset.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class AssetTests : public ::testing::Test
	{
	protected:

		AssetTests()
		{
		}

		~AssetTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Asset asset;
	};

	TEST_F(AssetTests, SetGetMetaData)
	{
		const nlohmann::json expectedMetaData = "{ \"string\": \"test string\" }";
		asset.setMetaData(expectedMetaData);
		const nlohmann::json actualMetaData = asset.getMetaData();

		ASSERT_EQ(expectedMetaData, actualMetaData);
	}

	TEST_F(AssetTests, CreateObjectAndLoadContentUsingMetaData)
	{
		File file;
		file.open(Paths::CurrentProjectRootPath() / "assets_finder_test_folder" / "Content" / "text_file.txt.asset", FileOpenMode::Read);
		ASSERT_TRUE(file.isOpen());

		const auto fileContent = file.readAll();
		const nlohmann::json metaData = nlohmann::json::parse(fileContent);
		file.close();

		asset.setMetaData(metaData);

		const std::filesystem::path contentFolderPath = Paths::CurrentProjectRootPath() / "assets_finder_test_folder";
		auto object = asset.createObject(contentFolderPath);
		ASSERT_TRUE(object);

		auto textAsset = dynamic_cast<TextAsset*>(object.get());
		ASSERT_TRUE(textAsset);

		const bool loadContentResult = textAsset->loadContentUsingMetaData(contentFolderPath);
		ASSERT_TRUE(loadContentResult);

		ASSERT_FALSE(textAsset->getText().empty());
	}
}