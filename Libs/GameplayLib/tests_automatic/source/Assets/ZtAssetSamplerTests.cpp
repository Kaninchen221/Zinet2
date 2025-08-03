#pragma once

#include <Zinet/Gameplay/Assets/ZtAssetSampler.hpp>
#include <Zinet/Gameplay/ZtEngineContext.hpp>

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{

	class AssetSamplerTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(AssetSamplerTests, Test)
	{
		EngineContext engineContext;
		auto& assetsStorage = engineContext.assetsStorage;
		assetsStorage.registerAssetClass<AssetSampler>("AssetSampler");

		engineContext.addSystem<SystemRenderer>();

		SystemRenderer::UseImGui = false;
		ASSERT_TRUE(engineContext.init());

		auto asset = assetsStorage.getAs<AssetSampler>("Content/Samplers/linear.sampler");
		ASSERT_TRUE(asset);

		// Sampler should be auto loaded
		ASSERT_TRUE(asset->isLoaded());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());

		engineContext.deinit();
		SystemRenderer::UseImGui = true;
	}
}