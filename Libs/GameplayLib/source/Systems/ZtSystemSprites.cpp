#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedSprite.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Core/ZtTimeLog.hpp"

namespace zt::gameplay
{
	void SystemSprites::update()
	{
		System::update();

		if (isDirty)
		{
			recreateTransformsBuffer();
			isDirty = false;
		}

		ZT_TIME_LOG(
			// TODO: Optimize this. We should update only changed transforms
			updateTransformsBufferData();
		);
	}

	void SystemSprites::addNode(const ObjectWeakHandle<Node>& node)
	{
		System::addNode(node);

		if (!Ensure(node.isValid()))
			return;

		ObjectWeakHandle<NodeInstancedSprite> sprite = node;
		if (!Ensure(sprite.isValid()))
			return;

		auto selfHandle = getSelf();
		if (!Ensure(selfHandle.isValid()))
			return;

		sprite->systemSprites = selfHandle.castTo<SystemSprites, false>();
		const uint32_t id = ids.emplace_back(static_cast<uint32_t>(ids.size()));
		sprite->id = id;
		transforms.emplace_back();

		isDirty = true;
	}

	vulkan_renderer::DescriptorInfo SystemSprites::getDescriptorInfo()
	{
		if (!transformsMatricesBuffer.isValid())
			return {};

		vulkan_renderer::DescriptorInfo result;

		result.buffersPacks.emplace_back().buffersPerType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].push_back(&transformsMatricesBuffer);

		if (!assetTexture.isValid() || !assetTexture->isLoaded())
		{
			Logger->error("AssetTexture is invalid");
			return result;
		}

		auto& sampler = assetTexture->sampler;
		if (!sampler.isValid() || !sampler->isLoaded())
		{
			Logger->error("Sampler of texture is invalid");
			return result;
		}

		result.texturesInfos =
		{
			vulkan_renderer::TextureInfo
			{
				.texture = &assetTexture->texture,
				.sampler = &sampler.get()->sampler,
				.shaderType = vulkan_renderer::ShaderType::Fragment
			}
		};

		return result;
	}

	void SystemSprites::recreateTransformsBuffer()
	{
		using namespace vulkan_renderer;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!Ensure(systemRenderer))
		{
			Logger->error("System renderer is invalid");
			return;
		}
		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& vma = rendererContext.getVMA();

		if (transformsMatricesBuffer.isValid())
		{
			transformsMatricesBuffer.destroy(vma);
		}

		transformsMatrices.clear();
		for (const auto& transform : transforms)
		{
			transformsMatrices.emplace_back(transform.getMatrix());
		}

		auto createInfo = Buffer::GetStorageBufferCreateInfo(transformsMatrices);
		transformsMatricesBuffer.create(vma, createInfo);

		auto& device = rendererContext.getDevice();
		device.setDebugName(transformsMatricesBuffer, "SystemSprites transforms matrices", VK_OBJECT_TYPE_BUFFER);
	}

	void SystemSprites::updateTransformsBufferData()
	{
		if (!Ensure(transformsMatricesBuffer.isValid()))
			return;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!Ensure(systemRenderer))
		{
			Logger->error("System renderer is invalid");
			return;
		}
		auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

		ZT_TIME_LOG(
			transformsMatricesBuffer.fillWithSTDContainer(vma, transformsMatrices);
		);
	}

	bool SystemSprites::deinit()
	{
		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!Ensure(systemRenderer))
		{
			Logger->error("System renderer is invalid");
			return false;
		}
		auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

		transformsMatricesBuffer.destroy(vma);

		initialized = false; 
		return true;
	}

}