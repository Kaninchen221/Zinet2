#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{

	class ShaderTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			assetStorage.registerAssetClass<Shader>();
			bool result = assetStorage.storeAssets();
			ASSERT_TRUE(result);
		}

		void TearDown() override
		{
		}

		core::AssetHandle<Shader> getAssetHandle()
		{
			auto asset = assetStorage.getAs<Shader>("Content/Shaders/shader.vert");
			if (!asset)
				return {};

			return asset;
		}

		core::AssetStorage assetStorage;
	};

	TEST_F(ShaderTests, Test)
	{
		auto assetHandle = getAssetHandle();
		ASSERT_TRUE(assetHandle);

		ASSERT_TRUE(assetHandle->load());
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
		auto assetHandle = getAssetHandle();
		ASSERT_TRUE(assetHandle);
		ASSERT_TRUE(assetHandle->load());

		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));

		vulkan_renderer::RendererContext rendererContext;
		ASSERT_TRUE(rendererContext.create(window));

		std::optional<vulkan_renderer::ShaderModule> shaderModule = assetHandle->createResource(rendererContext);
		ASSERT_TRUE(shaderModule);
		EXPECT_TRUE(shaderModule->isValid());

		shaderModule->destroy(rendererContext.getDevice());
		
		rendererContext.destroy();

		window.destroyWindow();

		wd::GLFW::Deinit();
	}
}