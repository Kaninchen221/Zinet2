#pragma once

#include <Zinet/Core/Assets/ZtAssetsStorage.hpp>

#include <Zinet/Gameplay/Assets/ZtAssetSampler.hpp>
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{
	class SamplerTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(SamplerTests, Test)
	{
		core::AssetsStorage assetsStorage;
		assetsStorage.registerAssetClass<Sampler>();

		bool result = assetsStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetsStorage.getAs<Sampler>("Content/Samplers/linear.sampler");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetsStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->getTypeString().empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->getTypeString().empty());
	}
}