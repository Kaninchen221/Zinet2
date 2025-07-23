#pragma once

#include "Zinet/VulkanRenderer/Assets/ZtAssetShader.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::vulkan_renderer::assets::tests
{

	class AssetShaderTests : public ::testing::Test
	{
	protected:

		AssetShaderTests()
		{
		}

		~AssetShaderTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(AssetShaderTests, Test)
	{
		core::assets::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<AssetShader>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto assetShader = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
		ASSERT_TRUE(assetShader);

		ASSERT_TRUE(assetShader->load(assetsStorage.assetsFinder.rootFolder));
		ASSERT_TRUE(assetShader->isLoaded());
		ASSERT_FALSE(assetShader->shaderCode.empty());

		assetShader->unload();
		ASSERT_FALSE(assetShader->isLoaded());
		ASSERT_TRUE(assetShader->shaderCode.empty());
	}
}