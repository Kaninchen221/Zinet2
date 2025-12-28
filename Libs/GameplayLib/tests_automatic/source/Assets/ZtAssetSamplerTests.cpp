#pragma once

#include <Zinet/Core/Assets/ZtAssetStorage.hpp>

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
		core::AssetStorage assetStorage;
		assetStorage.registerAssetClass<Sampler>();

		bool result = assetStorage.storeAssets();
		ASSERT_TRUE(result);

		auto asset = assetStorage.getAs<Sampler>("Content/Samplers/linear.sampler");
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load(assetStorage.getAssetsFinder().getRootPath()));
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->getTypeString().empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->getTypeString().empty());
	}
}