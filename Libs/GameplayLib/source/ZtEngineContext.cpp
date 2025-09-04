#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	EngineContext::EngineContext()
	{
		if (instance)
		{
			Ensure(false, "We should have only a one instance of the Engine Context");
		}

		instance = this;
	};

	bool EngineContext::init()
	{
		if (initialized && instance)
		{
			Ensure(false, "Init should be called only once and we should have only one instance of the EngineContext");
			return true;
		}

		instance = this;

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

		initialized = true;

		return true;
	}

	void EngineContext::loop()
	{
		// TODO: Poll events should be called by some system
		windowEvents.pollEvents();

		for (auto& phaseSystems : systemsPerUpdatePhase)
		{
			for (auto& system : phaseSystems)
			{
				system->update();
			}
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