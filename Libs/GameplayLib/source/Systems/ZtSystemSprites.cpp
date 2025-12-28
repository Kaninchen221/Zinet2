#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

namespace zt::gameplay
{
	namespace system
	{
		using namespace core;
		using namespace vulkan_renderer;

		void Sprites::Init(
			core::ecs::WorldCommands worldCommands, 
			core::ecs::ConstResource<core::AssetStorage> assetStorageRes)
		{
			auto samplerAsset = assetStorageRes->getAs<asset::Sampler>("Content/Samplers/linear.sampler");
			if (!samplerAsset)
			{
				Logger->error("Couldn't get sampler asset");
				return;
			}
			
			if (!samplerAsset->load(core::Paths::RootPath()))
			{
				Logger->error("Couldn't load sampler asset");
				return;
			}

			auto textureAsset = assetStorageRes->getAs<asset::Texture>("Content/Textures/default_texture.png");
			if (!textureAsset)
			{
				Logger->error("Couldn't get texture asset");
				return;
			}
			
			if (!textureAsset->load(core::Paths::RootPath()))
			{
				Logger->error("Couldn't load texture asset");
				return;
			}

			auto vertexShaderAsset = assetStorageRes->getAs<asset::Shader>("Content/Shaders/shader_sprites.vert");
			if (!vertexShaderAsset)
			{
				Logger->error("Couldn't get vertex shader asset");
				return;
			}
			
			if (!vertexShaderAsset->load(core::Paths::RootPath()))
			{
				Logger->error("Couldn't load vertex asset");
				return;
			}

			auto fragmentShaderAsset = assetStorageRes->getAs<asset::Shader>("Content/Shaders/shader_sprites.frag");
			if (!fragmentShaderAsset)
			{
				Logger->error("Couldn't get fragment shader asset");
				return;
			}
			
			if (!fragmentShaderAsset->load(core::Paths::RootPath()))
			{
				Logger->error("Couldn't load fragment asset");
				return;
			}

			// Spawn system::Sprites input info
			// TODO: Refactor it

			auto samplerAssetCopy = samplerAsset;
			auto textureAssetCopy = textureAsset;

			worldCommands.spawn(
				system::Sprites{},
				vulkan_renderer::GraphicsPipeline{},
				vulkan_renderer::DrawInfo{},
				ShaderAssetsPack{
					.vertexShaderAsset = vertexShaderAsset,
					.fragmentShaderAsset = fragmentShaderAsset
				},
				textureAssetCopy,
				samplerAssetCopy,
				SpritesBuffers{});
		}

		void Sprites::Update(
			core::ecs::WorldCommands,
			SpriteQuery sprites,
			SystemComponentsQuery systemComponents,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<CameraManager> cameraManagerRes,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes)
		{	
			// TODO: Refactor validation of resources
			// Put it in the world/schedule class?
			if (!rendererRes)
			{
				Logger->error("Renderer res is invalid");
				return;
			}

			if (!resourceStorageRes)
			{
				Logger->error("Resource Storage res is invalid");
				return;
			}

			if (!cameraManagerRes)
			{
				Logger->error("CameraManager res is invalid");
				return;
			}

			// Create graphics pipelines
			if (!systemComponents.isEmpty())
			{
				for ([[maybe_unused]] auto [label, graphicsPipeline, drawInfo, shaderAssetsPack, textureAsset, samplerAsset, buffers] : systemComponents)
				{
					auto& positionBuffer = buffers->position;
					auto& rotationBuffer = buffers->rotation;
					auto& scaleBuffer = buffers->scale;
					auto& cameraBuffer = buffers->camera;

					if (graphicsPipeline->isValid())
					{
						UpdateComponentBuffer(rendererRes, sprites.getComponentsPack<Position>(), positionBuffer);
						UpdateComponentBuffer(rendererRes, sprites.getComponentsPack<Rotation>(), rotationBuffer);
						UpdateComponentBuffer(rendererRes, sprites.getComponentsPack<Scale>(), scaleBuffer);

						UpdateCameraBuffer(rendererRes, cameraManagerRes, cameraBuffer);

						continue;
					}

					auto vertexShaderModule = resourceStorageRes->request<ShaderModule>(shaderAssetsPack->vertexShaderAsset);
					auto fragmentShaderModule = resourceStorageRes->request<ShaderModule>(shaderAssetsPack->fragmentShaderAsset);
					auto texture = resourceStorageRes->request<Texture>(*textureAsset);
					auto sampler = resourceStorageRes->request<Sampler>(*samplerAsset);

					if (!vertexShaderModule || !fragmentShaderModule || !texture || !sampler || !buffers)
					{
						Logger->debug("Skip graphics pipeline creation because one of renderer resources is not loaded");
						continue;
					}

					if (!CreateVertexBuffer(rendererRes, buffers->vertex))
					{
						Logger->error("Couldn't create vertex buffer");
						continue;
					}

					if (!CreateIndexBuffer(rendererRes, buffers->index))
					{
						Logger->error("Couldn't create index buffer");
						continue;
					}

					drawInfo->vertexBuffer = &buffers->vertex;
					drawInfo->indexBuffer = &buffers->index;
					drawInfo->indexCount = 6u;
					drawInfo->instances = static_cast<uint32_t>(sprites.getComponentCount() / sprites.getTypeCount());

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Position>(), positionBuffer, "Sprites Position Buffer"))
					{
						Logger->error("Couldn't create position buffer");
						continue;
					}

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Rotation>(), rotationBuffer, "Sprites Rotation Buffer"))
					{
						Logger->error("Couldn't create rotation buffer");
						continue;
					}

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Scale>(), scaleBuffer, "Sprites Scale Buffer"))
					{
						Logger->error("Couldn't create scale buffer");
						continue;
					}

					if (!CreateCameraBuffer(rendererRes, cameraBuffer))
					{
						Logger->error("Couldn't create camera buffer");
						continue;
					}

					vulkan_renderer::ShaderModules shaderModules
					{ 
						{ vulkan_renderer::ShaderType::Vertex, vertexShaderModule },
						{ vulkan_renderer::ShaderType::Fragment, fragmentShaderModule }
					};

					TextureInfo textureInfo
					{
						.texture = texture,
						.sampler = sampler,
						.shaderType = ShaderType::Fragment
					};

					// TODO: Refactor
					GraphicsPipelineCreateInfo createInfo
					{
						.rendererContext = rendererRes->getRendererContext(),
						.shaderModules = shaderModules,
						.descriptorInfos =
						{
							DescriptorInfo
							{
								.buffersPacks = 
								{
									vulkan_renderer::BuffersPack
									{
										.binding = 0,
										.buffersPerType =
										{
											{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, { &cameraBuffer } }
										}
									}
								},
								.texturesInfos = {},
							},
							DescriptorInfo
							{
								.buffersPacks =
								{
									vulkan_renderer::BuffersPack
									{
										.binding = 0,
										.buffersPerType =
										{
											{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, { &positionBuffer } }
										}
									},
									vulkan_renderer::BuffersPack
									{
										.binding = 1,
										.buffersPerType =
										{
											{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, { &rotationBuffer } }
										}
									},
									vulkan_renderer::BuffersPack
									{
										.binding = 2,
										.buffersPerType =
										{
											{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, { &scaleBuffer } }
										}
									}
								},
								.texturesInfos{ textureInfo }
							}
						},
						.descriptorSetsCount = rendererRes->getRendererContext().getDisplayImagesCount()
					};

					graphicsPipeline->create(createInfo);

					auto& device = rendererRes->getRendererContext().getDevice();
					device.setDebugName(graphicsPipeline->getPipeline(), "Sprites Graphics Pipeline", VK_OBJECT_TYPE_PIPELINE);
					device.setDebugName(graphicsPipeline->getPipelineLayout(), "Sprites Graphics Pipeline Layout", VK_OBJECT_TYPE_PIPELINE_LAYOUT);
				}
			}
		}

		void Sprites::Deinit(
			core::ecs::WorldCommands worldCommands,
			SystemComponentsQuery systemComponents,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes
		)
		{
			if (!rendererRes)
			{
				worldCommands.addResource(ExitReason{ "Expected rendererRes" });
				return;
			}

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

			for ([[maybe_unused]] auto [label, graphicsPipeline, drawInfo, shaderAssetsPack, textureAsset, samplerAsset, buffers] : systemComponents)
			{
				buffers->destroy(vma);

				graphicsPipeline->destroy(rendererContext);
			}
		}

		bool Sprites::CreateVertexBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, vulkan_renderer::Buffer& buffer)
		{
			if (buffer)
			{
				Logger->warn("Passed a valid buffer");
				return false;
			}

			const DrawInfo::Vertices vertices = {
				{{-0.5f,  0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 0.f}},
				{{0.5f,   0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 0.f}},
				{{0.5f,  -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}},
				{{-0.5f, -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 1.f}}
			};

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();
			auto& device = rendererContext.getDevice();

			const auto createInfo = Buffer::GetVertexBufferCreateInfo(vertices);
			if (!buffer.create(vma, createInfo))
			{
				Logger->info("Couldn't create vertex buffer");
				return false;
			}
				
			if (!buffer.fillWithSTDContainer(vma, vertices))
			{
				Logger->info("Couldn't fill vertex buffer");
				return false;
			}

			device.setDebugName(buffer, "Sprites vertex buffer", VK_OBJECT_TYPE_BUFFER);

			return true;
		}

		bool Sprites::CreateIndexBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, vulkan_renderer::Buffer& buffer)
		{
			if (buffer)
			{
				Logger->warn("Passed a valid buffer");
				return false;
			}

			const DrawInfo::Indices indices =
			{
				0, 1, 2,
				2, 3, 0
			};

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();
			auto& device = rendererContext.getDevice();

			const auto createInfo = Buffer::GetIndexBufferCreateInfo(indices);
			if (!buffer.create(vma, createInfo))
			{
				Logger->info("Couldn't create index buffer");
				return false;
			}

			if (!buffer.fillWithSTDContainer(vma, indices))
			{
				Logger->info("Couldn't fill index buffer");
				return false;
			}

			device.setDebugName(buffer, "Sprites index buffer", VK_OBJECT_TYPE_BUFFER);

			return true;
		}

		bool Sprites::CreateCameraBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, vulkan_renderer::Buffer& buffer)
		{
			using namespace core;
			using namespace vulkan_renderer;

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();
			auto& device = rendererContext.getDevice();

			if (buffer)
			{
				Logger->warn("Passed a valid buffer");
				return false;
			}

			VkDeviceSize bufferSize = sizeof(Camera::MatrixT) * 2; // View + Perspective matrices

			VkBufferCreateInfo createInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = bufferSize,
				.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};

			if (!buffer.create(vma, createInfo))
			{
				Logger->error("Couldn't create transform buffer");
				return false;
			}

			device.setDebugName(buffer, "Sprites camera buffer", VK_OBJECT_TYPE_BUFFER);

			return buffer;
		}

		bool Sprites::UpdateCameraBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, 
			core::ecs::ConstResource<CameraManager> cameraManagerRes, 
			vulkan_renderer::Buffer& buffer)
		{
			const auto& rendererContext = rendererRes->getRendererContext();
			const auto& vma = rendererContext.getVMA();

			const auto& camera = cameraManagerRes->getCamera();

			auto viewMatrix = camera.getViewMatrix();
			auto perspectiveMatrix = camera.getPerspectiveMatrix();

			if (!buffer.fillWithObject(vma, viewMatrix))
				return false;

			if (!buffer.fillWithObject(vma, perspectiveMatrix, sizeof(Camera::MatrixT)))
				return false;

			return true;
		}

	}
}