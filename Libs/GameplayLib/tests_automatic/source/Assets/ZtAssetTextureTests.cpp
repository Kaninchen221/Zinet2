#pragma once

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <Zinet/Gameplay/Assets/ZtAssetTexture.hpp>
#include <Zinet/Gameplay/ZtEngineContext.hpp>
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{

	class AssetTextureTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(AssetTextureTests, Test)
	{
		EngineContext engineContext;
		auto& assetsStorage = engineContext.getAssetsStorage();
		assetsStorage.registerAssetClass<AssetTexture>();

		engineContext.addSystem<SystemRenderer>("SystemRenderer");

		SystemRenderer::UseImGui = false;
		ASSERT_TRUE(engineContext.init());

		auto asset = assetsStorage.getAs<AssetTexture>("Content/Textures/image.png");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());

		engineContext.deinit();
		SystemRenderer::UseImGui = true;
	}
}