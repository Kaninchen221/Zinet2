#include "Zinet/Gameplay/Systems/ZtSystemFlipBooks.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeInstancedFlipBook.hpp"

#include <ranges>

namespace zt::gameplay
{
	void SystemFlipBooks::update()
	{
		SystemSprites::update();

		auto elapsedTime = clock.getElapsedTime().getAsSeconds();
		for (size_t i = 0; i < ids.size(); ++i)
		{
			auto& currentIndex = currentFramesIndices[i];
			auto& frames = flipBooksFrames[i];
			auto& currentFrameCoords = currentFramesCoords[i];
			auto& frameTimeout = framesTimeouts[i];

			frameTimeout -= elapsedTime;
			if (frameTimeout > 0.f)
				continue;

			++currentIndex;
			if (currentIndex >= frames.size())
				currentIndex = 0;

			auto& currentFrame = frames[currentIndex];
			frameTimeout = currentFrame.time;
			currentFrameCoords = currentFrame.textureCoords;

		}
		clock.restart();

		{ // Update buffer data
			auto& engineContext = EngineContext::Get();
			auto systemRenderer = engineContext.getSystem<SystemRenderer>();
			if (!Ensure(systemRenderer))
			{
				Logger->error("System renderer is invalid");
				return;
			}
			auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

			if (frameTexCoordsBuffer.isValid())
			{
				if (!frameTexCoordsBuffer.fillWithSTDContainer(vma, currentFramesCoords))
				{
					Logger->error("Failed filling frame tex coords buffer with data");
				}
			}
		}

		if (!frameTexCoordsBuffer.isValid())
		{
			recreateFrameTexCoordsBuffer();
		}
	}

	bool SystemFlipBooks::init()
	{
		return SystemSprites::init();
	}

	bool SystemFlipBooks::deinit()
	{
		auto systemSpritesDeinit = SystemSprites::deinit();

		if (frameTexCoordsBuffer.isValid())
		{
			auto& engineContext = EngineContext::Get();
			auto systemRenderer = engineContext.getSystem<SystemRenderer>();
			if (!Ensure(systemRenderer))
			{
				Logger->error("System renderer is invalid");
				return false;
			}
			auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

			frameTexCoordsBuffer.destroy(vma);
		}

		return systemSpritesDeinit;
	}

	void SystemFlipBooks::addNode(const ObjectWeakHandle<Node>& node)
	{
		System::addNode(node);

		if (!Ensure(node.isValid()))
			return;

		ObjectWeakHandle<NodeInstancedFlipBook> flipBook = node;
		if (!Ensure(flipBook.isValid()))
			return;

		auto selfHandle = getSelf();
		if (!Ensure(selfHandle.isValid()))
			return;

		flipBook->systemFlipBooks = selfHandle.castTo<SystemFlipBooks, false>();
		const uint32_t id = ids.emplace_back(static_cast<uint32_t>(ids.size()));
		flipBook->id = id;
		transforms.emplace_back();
		flipBooksFrames.emplace_back();
		currentFramesIndices.emplace_back(-1);
		currentFramesCoords.emplace_back();
		framesTimeouts.emplace_back(0.f);

		isDirty = true;
	}

	vulkan_renderer::DescriptorInfo SystemFlipBooks::getDescriptorInfo()
	{
		auto result = SystemSprites::getDescriptorInfo();

		auto& buffersPack = result.buffersPacks.emplace_back();
		buffersPack.buffersPerType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].push_back(&frameTexCoordsBuffer);
		buffersPack.binding = 1;

		return result;
	}

	void SystemFlipBooks::recreateFrameTexCoordsBuffer()
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
		auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

		currentFramesCoords.clear();
		for (const auto& [frames, currentIndex] : std::views::zip(flipBooksFrames, currentFramesIndices))
		{
			currentFramesCoords.emplace_back(frames[currentIndex].textureCoords);
		}

		if (frameTexCoordsBuffer.isValid())
		{
			frameTexCoordsBuffer.destroy(vma);
		}

		auto createInfo = Buffer::GetStorageBufferCreateInfo(currentFramesCoords);
		frameTexCoordsBuffer.create(vma, createInfo);

		auto& device = rendererContext.getDevice();
		device.setDebugName(frameTexCoordsBuffer, "SF frameTexCoordsBuffer", VK_OBJECT_TYPE_BUFFER);
	}

}