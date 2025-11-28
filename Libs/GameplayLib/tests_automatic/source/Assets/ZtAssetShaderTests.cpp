#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{

	class ShaderTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(ShaderTests, Test)
	{
		core::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<Shader>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetsStorage.getAs<Shader>("Content/Shaders/shader.vert");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->getText().empty());
		ASSERT_FALSE(asset->getCompileResult().empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->getText().empty());
		ASSERT_TRUE(asset->getCompileResult().empty());

	}
}