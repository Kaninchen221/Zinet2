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

		void Sprites::Init()
		{
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
				for ([[maybe_unused]] auto [label, graphicsPipeline, shaderAssetsPack, textureAsset, samplerAsset, buffers] : systemComponents)
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
						Logger->trace("Skip graphics pipeline creation because one of assets is not loaded");
						continue;
					}

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Position>(), positionBuffer))
					{
						Logger->error("Couldn't create position buffer");
						continue;
					}

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Rotation>(), rotationBuffer))
					{
						Logger->error("Couldn't create rotation buffer");
						continue;
					}

					if (!CreateComponentBuffer(rendererRes, sprites.getComponentsPack<Scale>(), scaleBuffer))
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

			for ([[maybe_unused]] auto [label, graphicsPipeline, shaderAssetsPack, textureAsset, samplerAsset, buffers] : systemComponents)
			{
				buffers->position.destroy(vma);
				buffers->rotation.destroy(vma);
				buffers->scale.destroy(vma);
				buffers->camera.destroy(vma);

				graphicsPipeline->destroy(rendererContext);
			}
		}

		bool Sprites::CreateCameraBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, vulkan_renderer::Buffer& buffer)
		{
			using namespace core;
			using namespace vulkan_renderer;

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

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