#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/ZtExitReason.hpp"
#include "Zinet/Core/ZtTimeLog.hpp"

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

			auto rendererRes = world.addResource(VulkanRenderer{});
			ASSERT_TRUE(rendererRes);
			ASSERT_TRUE(rendererRes->init(window));

			auto resourceStorageRes = world.addResource(ResourceStorage{});
			ASSERT_TRUE(resourceStorageRes);

			auto cameraManagerRes = world.addResource(CameraManager{});
			ASSERT_TRUE(cameraManagerRes);

			auto& assetStorage = *world.addResource(AssetStorage{});
			assetStorage.registerAssetClass<asset::Texture>();
			assetStorage.registerAssetClass<asset::Shader>();
			assetStorage.registerAssetClass<asset::Sampler>();
			assetStorage.storeAssets();
		}

		void TearDown() override
		{
			auto rendererRes = world.getResource<VulkanRenderer>();
			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

			for ([[maybe_unused]] auto [label, graphicsPipeline, drawInfo, shaderAssetsPack, textureAsset, samplerAsset, buffers] : Sprites::SystemComponentsQuery{ world })
			{
				buffers->destroy(vma);

				graphicsPipeline->destroy(rendererContext);
			}

			auto resourceStorageRes = world.getResource<vulkan_renderer::ResourceStorage>();
			resourceStorageRes->clear(rendererContext);

			ASSERT_TRUE(rendererRes);
			rendererRes->deinit();

			window.destroyWindow();
		}

		wd::GLFW glfw;
		wd::Window window;
		ecs::World world;
		ecs::Schedule schedule;
	};

	TEST_F(SpritesTests, Test)
	{
		// Spawn Sprites
		const size_t spritesCount = 10;
		for (size_t index = 0; index < spritesCount; ++index)
		{
			world.spawn(Sprite{}, zt::Position{ { 0, 0, 0 } }, zt::Rotation{ 0 }, zt::Scale{ { 1, 1, 1 } });
			// TODO: Apply a random: position, scale and rotation
			// TODO: Maybe add some way to define Entity to reduce redundancy of defining Entity types
		}

		{ // Init
			schedule.runOneSystemOnce(Sprites{}, Sprites::Init, world);

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		{ // Update
			// TODO: Sprites
			// 1. Handle situation when we remove a sprite
			// 

			// We need to run the update method at least two times because the system is requesting renderer resources
			for (size_t i = 0; i < 4; i++)
			{
				ZT_TIME_LOG(schedule.runOneSystemOnce(Sprites{}, Sprites::Update, world));

				auto rendererRes = world.getResource<VulkanRenderer>();
				auto resourceStorageRes = world.getResource<ResourceStorage>();
				resourceStorageRes->createResources(rendererRes->getRendererContext());
			}
			
			system::Sprites::SystemComponentsQuery query{ world };
			ASSERT_EQ(query.getComponentCount(), 7); // Sanity check
			for ([[maybe_unused]] auto [label, graphicsPipeline, drawInfo, shaderAssetsPack, textureAsset, samplerAsset, buffers] : query)
			{
				ASSERT_TRUE(buffers->index);
				ASSERT_TRUE(buffers->vertex);
				ASSERT_TRUE(buffers->camera);
				ASSERT_TRUE(buffers->position);
				ASSERT_TRUE(buffers->scale);
				ASSERT_TRUE(buffers->rotation);

				ASSERT_TRUE(graphicsPipeline);
				EXPECT_TRUE(graphicsPipeline->isValid());

				{
					ASSERT_TRUE(drawInfo->indexBuffer);
					ASSERT_TRUE(drawInfo->vertexBuffer);
					ASSERT_NE(drawInfo->indexCount, 0);
					ASSERT_NE(drawInfo->instances, 0);

					auto rendererRes = world.getResource<VulkanRenderer>();
					auto& renderer = *rendererRes;

					ASSERT_TRUE(renderer.nextImage());

					renderer.startRecordingDrawCommands();

					renderer.beginRenderPass(renderer.getRendererContext().getRenderPass());

					renderer.draw(*graphicsPipeline, *drawInfo);

					renderer.endRenderPass();

					renderer.endRecordingDrawCommands();

					ASSERT_TRUE(renderer.submitCurrentDisplayImage());

					ASSERT_TRUE(renderer.displayCurrentImage());
				}
			}

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}

		auto rendererRes = world.getResource<VulkanRenderer>();
		rendererRes->waitForCompleteDrawing();

		{ // Deinit
			schedule.runOneSystemOnce(Sprites{}, Sprites::Deinit, world);

			if (auto exitReason = world.getResource<ExitReason>())
				FAIL() << exitReason->reason;
		}
	}
}