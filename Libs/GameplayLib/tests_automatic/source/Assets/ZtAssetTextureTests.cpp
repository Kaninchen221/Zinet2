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
			assetStorage.registerAssetClass<Texture>();
			bool result = assetStorage.storeAssets();
			ASSERT_TRUE(result);
		}

		void TearDown() override
		{
		}

		core::AssetHandle<asset::Texture> getShaderAssetHandle()
		{
			auto asset = assetStorage.getAs<Texture>("Content/Textures/default_texture.png");
			if (!asset)
				return {};

			return asset;
		}

		core::AssetStorage assetStorage;
	};

	TEST_F(TextureTests, Test)
	{
		auto assetHandle = getShaderAssetHandle();
		ASSERT_TRUE(assetHandle);

		ASSERT_TRUE(assetHandle->load(core::Paths::RootPath()));
		ASSERT_TRUE(assetHandle->isLoaded());

		ASSERT_TRUE(assetHandle->getImage().getData());

		assetHandle->unload();
		ASSERT_FALSE(assetHandle->isLoaded());
	}

	TEST_F(TextureTests, CreateResourceTest)
	{
		auto assetHandle = getShaderAssetHandle();
		ASSERT_TRUE(assetHandle);
		ASSERT_TRUE(assetHandle->load(core::Paths::RootPath()));

		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));

		vulkan_renderer::RendererContext rendererContext;
		ASSERT_TRUE(rendererContext.create(window));
		
		std::optional<vulkan_renderer::Texture> texture = assetHandle->createResource(rendererContext);
		EXPECT_TRUE(texture);

		if (texture)
		{
			auto& device = rendererContext.getDevice();
			auto& vma = rendererContext.getVMA();

			texture->destroy(device, vma);
		}

		rendererContext.destroy();

		window.destroyWindow();

		wd::GLFW::Deinit();
	}
}