#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

namespace zt::gameplay::system::tests
{
	using namespace zt::core;
	using namespace zt::vulkan_renderer;

	class SpritesTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(window.create(200, 200));
		}

		void TearDown() override
		{
			auto rendererRes = world.getResource<VulkanRenderer>();
			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

			ecs::Query<Sprites::Data> query{ world };
			for (auto [data] : query)
			{
				auto& buffer = data->transformBuffer;
				if (buffer && buffer.isValid())
				{
					buffer.destroy(vma);
				}
			}

			auto resourceStorageRes = world.getResource<vulkan_renderer::ResourceStorage>();
			resourceStorageRes->clear(rendererContext);

			ASSERT_TRUE(rendererRes);
			rendererRes->deinit();

			window.destroyWindow();
		}

		using QuerySpritesDataT = ecs::Query<Sprites::Data>;

		wd::GLFW glfw;
		wd::Window window;
		ecs::World world;
		ecs::Schedule schedule;
	};

	// TODO: Sprites (Later)
	// - Parametrize shaders
	// - Parametrize used texture (or add something like atlas of textures?)
	// - Parametrize used texture sampler
	TEST_F(SpritesTests, Test)
	{
		auto rendererRes = world.addResource(VulkanRenderer{});
		ASSERT_TRUE(rendererRes);
		ASSERT_TRUE(rendererRes->init(window));

		auto resourceStorageRes = world.addResource(ResourceStorage{});
		ASSERT_TRUE(resourceStorageRes);

		AssetStorage assetStorage;
		assetStorage.registerAssetClass<asset::Texture>();
		assetStorage.registerAssetClass<asset::Shader>();
		assetStorage.registerAssetClass<asset::Sampler>();
		assetStorage.storeAssets();

		world.addResource(assetStorage);

		// Spawn Sprites
		const size_t spritesCount = 10;
		for (size_t index = 0; index < spritesCount; ++index)
			world.spawn(Sprite{}, Transform{});	

		{ // Init
			schedule.runOneSystemOnce(Sprites{}, Sprites::Init, world);

			QuerySpritesDataT query{ world };
			ASSERT_EQ(query.getComponentsCount(), 1);

			for (auto [data] : query)
			{
				auto& transformBuffer = data->transformBuffer;
				auto& textureAssetHandle = data->texture;
				auto& samplerAssetHandle = data->sampler;

				ASSERT_TRUE(transformBuffer);
				ASSERT_TRUE(transformBuffer.isValid());

				const size_t expectedSize = spritesCount * sizeof(Transform);
				ASSERT_EQ(expectedSize, transformBuffer.getSize());

				ASSERT_TRUE(textureAssetHandle);
				ASSERT_TRUE(textureAssetHandle.isValid());

				auto textureAsset = textureAssetHandle.get();
				ASSERT_TRUE(textureAsset->isLoaded());

				ASSERT_TRUE(samplerAssetHandle);
				ASSERT_TRUE(samplerAssetHandle.isValid());
				auto samplerAsset = samplerAssetHandle.get();
				ASSERT_TRUE(samplerAsset->isLoaded());
			}

			// TODO: Sprites (now)
			// Descriptors
			// GraphicsPipeline
			// RenderDrawData
			// Fill Transform Buffer with data

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		resourceStorageRes->createResources(rendererRes->getRendererContext());

		{ // Update
			schedule.runOneSystemOnce(Sprites{}, Sprites::Update, world);

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		{ // Deinit
			schedule.runOneSystemOnce(Sprites{}, Sprites::Deinit, world);

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}
	}
}