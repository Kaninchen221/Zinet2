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

		auto& assetStorage = *world.addResource(AssetStorage{});
		assetStorage.registerAssetClass<asset::Texture>();
		assetStorage.registerAssetClass<asset::Shader>();
		assetStorage.registerAssetClass<asset::Sampler>();
		assetStorage.storeAssets();

		auto samplerAsset = assetStorage.getAs<asset::Sampler>("Content/Samplers/linear.sampler");
		ASSERT_TRUE(samplerAsset);

		auto textureAsset = assetStorage.getAs<asset::Texture>("Content/Textures/default_texture.png");
		ASSERT_TRUE(textureAsset);

		auto vertexShaderAsset = assetStorage.getAs<asset::Shader>("Content/Shaders/shader_sprites.vert");
		ASSERT_TRUE(vertexShaderAsset);

		auto fragmentShaderAsset = assetStorage.getAs<asset::Shader>("Content/Shaders/shader_sprites.frag");
		ASSERT_TRUE(fragmentShaderAsset);

		// Spawn Sprites
		const size_t spritesCount = 10;
		for (size_t index = 0; index < spritesCount; ++index)
		{
			auto samplerAssetCopy = samplerAsset;
			auto textureAssetCopy = textureAsset;
			auto vertexShaderAssetCopy = vertexShaderAsset;
			auto fragmentShaderAssetCopy = fragmentShaderAsset;
			world.spawn(Sprite{}, Transform{}, samplerAssetCopy, textureAssetCopy, vertexShaderAssetCopy, fragmentShaderAssetCopy);
			// TODO: Maybe add some way to define Entity to reduce redundancy of defining Entity types
		}

		{ // Init
			schedule.runOneSystemOnce(Sprites{}, Sprites::Init, world);

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		resourceStorageRes->createResources(rendererRes->getRendererContext());

		{ // Update
			schedule.runOneSystemOnce(Sprites{}, Sprites::Update, world);

			// TODO: Sprites
			// 1.
			// Sort sprites using their components to create graphics pipelines only once per unique combination?
			// Or handle only one combination for now
			// 
			// 2. Create descriptors
			// 3. Create graphics pipelines
			// 4. Expect a valid GraphicsPipeline in test
			// 5. Create and expect render draw data
			// 6. Update transform buffers

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