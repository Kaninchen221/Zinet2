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
			assetStorage.registerAssetClass<Sampler>();
			bool result = assetStorage.storeAssets();
			ASSERT_TRUE(result);
		}

		void TearDown() override
		{
		}

		core::AssetHandle<Sampler> getAssetHandle()
		{
			auto asset = assetStorage.getAs<Sampler>("Content/Samplers/linear.sampler");
			if (!asset)
				return {};

			return asset;
		}

		core::AssetStorage assetStorage;
	};

	TEST_F(SamplerTests, Test)
	{
		auto asset = getAssetHandle();
		ASSERT_TRUE(asset);

		ASSERT_TRUE(asset->load());
		ASSERT_TRUE(asset->isLoaded());
		ASSERT_FALSE(asset->getTypeString().empty());

		asset->unload();
		ASSERT_FALSE(asset->isLoaded());
		ASSERT_TRUE(asset->getTypeString().empty());
	}

	TEST_F(SamplerTests, CreateResourceTest)
	{
		auto asset = getAssetHandle();
		ASSERT_TRUE(asset);
		ASSERT_TRUE(asset->load());

		wd::GLFW::Init();
		vulkan_renderer::RendererContext rendererContext;
		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));
		ASSERT_TRUE(rendererContext.create(window));

		std::optional<vulkan_renderer::Sampler> sampler = asset->createResource(rendererContext);
		ASSERT_TRUE(sampler);
		EXPECT_TRUE(sampler->isValid());

		sampler->destroy(rendererContext);

		rendererContext.destroy();
		window.destroyWindow();
		wd::GLFW::Deinit();
	}
}