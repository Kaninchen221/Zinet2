#pragma once

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

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

	TEST_F(AssetsStorageTests, StoreAssetsTest)
	{
		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto validAsset = assetsStorage.get("Content/placeholder.txt");
		ASSERT_TRUE(validAsset);

		auto invalidAsset = assetsStorage.get("invalid path");
		ASSERT_FALSE(invalidAsset);
	}
}