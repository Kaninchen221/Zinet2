#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

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
		SystemRenderer::SetUseImGui(false);

		core::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<AssetShader>();

		EngineContext engineContext;
		engineContext.addSystem<SystemWindow>("SystemWindow");
		engineContext.addSystem<SystemRenderer>("SystemRenderer");

		engineContext.init();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetsStorage.getAs<AssetShader>("Content/Shaders/shader.vert");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->getText().empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->getText().empty());

		engineContext.deinit();
		SystemRenderer::SetUseImGui(true);
	}
}