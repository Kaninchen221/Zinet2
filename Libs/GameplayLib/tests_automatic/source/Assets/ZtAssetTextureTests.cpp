#pragma once

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <Zinet/Gameplay/Assets/ZtAssetTexture.hpp>
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{

	class TextureTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(TextureTests, Test)
	{
		core::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<Texture>();

		assetsStorage.storeAssets();

		auto asset = assetsStorage.getAs<Texture>("Content/Textures/image.png");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());

		ASSERT_TRUE(asset->getImage().getData());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());

	}
}