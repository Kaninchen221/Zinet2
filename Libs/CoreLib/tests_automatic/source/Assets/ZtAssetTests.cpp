#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class AssetTests : public ::testing::Test
	{
	protected:

		Asset asset;
	};

	TEST_F(AssetTests, Pass)
	{
	}

	class AssetHandleTests : public ::testing::Test
	{
	protected:

		Asset asset;
		AssetHandle<Asset> assetHandle;
	};

	TEST_F(AssetHandleTests, Pass)
	{
		ASSERT_FALSE(assetHandle.isValid());
		ASSERT_FALSE(assetHandle);
		ASSERT_FALSE(assetHandle.get());
		ASSERT_FALSE(assetHandle.operator ->());

		assetHandle = AssetHandle<Asset>(&asset);
		ASSERT_TRUE(assetHandle.isValid());
		ASSERT_TRUE(assetHandle);
		ASSERT_TRUE(assetHandle.get());
		ASSERT_TRUE(assetHandle.operator ->());
		ASSERT_EQ(&assetHandle.operator *(), assetHandle.get());

		AssetHandle<Asset> second = assetHandle;
		ASSERT_TRUE(second.isValid());
		ASSERT_TRUE(assetHandle.isValid());

		second.invalidate();
		ASSERT_FALSE(second.isValid());
		second = assetHandle;
		ASSERT_TRUE(second.isValid());

		assetHandle.invalidate();
		assetHandle = std::move(second);
		ASSERT_FALSE(second.isValid());
		ASSERT_TRUE(assetHandle.isValid());

		AssetHandle<Asset> third = std::move(assetHandle);
		ASSERT_TRUE(third.isValid());
	}
}