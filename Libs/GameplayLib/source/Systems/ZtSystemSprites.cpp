#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedSprite.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

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

		updateTransformsBufferData();
	}

	void SystemSprites::addNode(const ObjectWeakHandle<Node>& node)
	{
		System::addNode(node);

		if (!Ensure(node.isValid()))
			return;

		ObjectWeakHandle<NodeInstancedSprite> sprite = node;
		if (!Ensure(sprite.isValid()))
			return;

		sprite->systemSprites = getSelf().castTo<SystemSprites, false>();
		const uint32_t id = ids.emplace_back(static_cast<uint32_t>(ids.size()));
		sprite->id = id;
		transforms.emplace_back();

		isDirty = true;
	}

	vulkan_renderer::DescriptorInfo SystemSprites::getDescriptorInfo()
	{
		if (!transformsMatricesBuffer.isValid())
			return {};

		return vulkan_renderer::DescriptorInfo
		{
			.buffersPerType =
			{
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, { &transformsMatricesBuffer } }
			}
		};
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
		auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

		if (transformsMatricesBuffer.isValid())
		{
			transformsMatricesBuffer.destroy(vma);
		}

		for (const auto& transform : transforms)
		{
			transformsMatrices.emplace_back(transform.getMatrix());
		}

		auto createInfo = Buffer::GetStorageBufferCreateInfo(transformsMatrices);
		transformsMatricesBuffer.create(vma, createInfo);
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

		transformsMatricesBuffer.fillWithSTDContainer(vma, transformsMatrices);
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