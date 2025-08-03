#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{

	class AssetShaderTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(AssetShaderTests, Test)
	{
		SystemRenderer::UseImGui = false;

		core::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<AssetShader>("AssetShader");

		EngineContext engineContext;
		engineContext.addSystem<SystemRenderer>("SystemRenderer");

		engineContext.init();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.assetsFinder.getRootPath()));
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->text.empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->text.empty());

		engineContext.deinit();
		SystemRenderer::UseImGui = true;
	}
}