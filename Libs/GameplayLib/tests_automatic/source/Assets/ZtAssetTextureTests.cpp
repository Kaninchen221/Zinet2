#pragma once

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

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
		core::AssetStorage assetStorage;
		assetStorage.registerAssetClass<Texture>();

		assetStorage.storeAssets();

		auto asset = assetStorage.getAs<Texture>("Content/Textures/image.png");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());

		ASSERT_TRUE(asset->getImage().getData());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());

	}
}