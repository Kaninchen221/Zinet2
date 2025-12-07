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
			assetsStorage.registerAssetClass<Shader>();
			bool result = assetsStorage.storeAssets();
			ASSERT_TRUE(result);
		}

		void TearDown() override
		{
		}

		core::AssetHandle<asset::Shader> getShaderAssetHandle()
		{
			auto asset = assetsStorage.getAs<Shader>("Content/Shaders/shader.vert");
			if (!asset)
				return {};

			return asset;
		}

		core::AssetsStorage assetsStorage;
	};

	TEST_F(ShaderTests, Test)
	{
		auto assetHandle = getShaderAssetHandle();
		ASSERT_TRUE(assetHandle);

		ASSERT_TRUE(assetHandle->load(core::Paths::RootPath()));
		ASSERT_TRUE(assetHandle->isLoaded());
		ASSERT_FALSE(assetHandle->getText().empty());
		ASSERT_FALSE(assetHandle->getCompileResult().empty());

		assetHandle->unload();
		ASSERT_FALSE(assetHandle->isLoaded());
		ASSERT_TRUE(assetHandle->getText().empty());
		ASSERT_TRUE(assetHandle->getCompileResult().empty());
	}

	TEST_F(ShaderTests, CreateResourceTest)
	{
		auto assetHandle = getShaderAssetHandle();
		ASSERT_TRUE(assetHandle);
		ASSERT_TRUE(assetHandle->load(core::Paths::RootPath()));

		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));

		vulkan_renderer::RendererContext rendererContext;
		ASSERT_TRUE(rendererContext.create(window));

		std::optional<vulkan_renderer::ShaderModule> shaderModule = assetHandle->createResource(rendererContext);
		EXPECT_TRUE(shaderModule);

		if (shaderModule)
			shaderModule->destroy(rendererContext.getDevice());
		
		rendererContext.destroy();

		window.destroyWindow();

		wd::GLFW::Deinit();
	}
}