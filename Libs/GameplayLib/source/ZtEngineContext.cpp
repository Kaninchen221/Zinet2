#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool EngineContext::init()
	{
		if (initialized)
			return true;

		wd::GLFW::Init(false);

		if (!window.create())
			return false;

		for (auto& system : systems)
		{
			system->init();
		}

		if (!assetsStorage.storeAssets())
			return false;

		rootNode = CreateObject<Node>("RootNode");
		rootNode->setSaveable(true);

		instance = this;
		initialized = true;

		return true;
	}

	void EngineContext::loop()
	{
		windowEvents.pollEvents();

		for (auto& system : systems)
		{
			system->update();
		}
	}

	void EngineContext::deinit()
	{
		if (!initialized)
			return;

		for (auto& system : systems)
		{
			system->waitCompleteJobs();
		}

		destroyNodes(rootNode);

		assetsStorage.unloadAssets();

		for (auto& system : systems)
		{
			if (!system->deinit())
			{
				Logger->error("System: {} deinit failed", system->getClassName());
			}
		}

		window.destroyWindow();
		wd::GLFW::Deinit();

		instance = nullptr;
	}

	EngineContext::~EngineContext() noexcept
	{
		if (instance)
		{
			Logger->error("EngineContext must be manually deinitialized");
		}
	}

	void EngineContext::destroyNodes(ObjectHandle<Node>& node)
	{
		if (!node)
			return;

		for (auto& child : node->getChildren())
		{
			destroyNodes(child);
		}
		node.destroy();
	}

}