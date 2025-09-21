#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	EngineContext& EngineContext::Get() noexcept
	{
#	if ZINET_DEBUG
		if (!Instance)
		{
			Logger->critical("Invalid instance");
			Terminate();
		}
#	endif

		return *Instance;
	}

	EngineContext::EngineContext()
	{
		if (Instance)
		{
			Ensure(false, "We should have only a one instance of the Engine Context");
		}

		Instance = this;
	};

	bool EngineContext::init()
	{
		if (initialized && Instance)
		{
			Ensure(false, "Init should be called only once and we should have only one instance of the EngineContext");
			return true;
		}

		Logger->info("Initialize engine context");

		Instance = this;

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
		renderingThread.runAsync();
		mainThread.runSync();
	}

	bool EngineContext::isLooping() const
	{
		bool renderingThreadRunning = renderingThread.isRunning();
		bool mainThreadRunning = mainThread.isRunning();

		return renderingThreadRunning && mainThreadRunning;
	}

	void EngineContext::stopLooping()
	{
		Logger->info("Stop looping engine context");

		if (!renderingThread.isRunning() || !mainThread.isRunning())
			return;

		renderingThread.stop();
		mainThread.stop();

		renderingThread.wait();
		mainThread.wait();
	}

	void EngineContext::deinit()
	{
		if (!initialized)
			return;

		Logger->info("Deinitialize engine context");

		// TODO: waitCompleteJobs or wait for engine threads?
		for (auto& system : systems)
		{
			system->waitCompleteJobs();
		}

		assetsStorage.unloadAssets();

		stopLooping();

		destroyNodes(rootNode);

		// Deinit systems in reverse order like RAII
		for (auto systemIt = systems.rbegin(); systemIt != systems.rend(); ++systemIt)
		{
			auto& system = *systemIt;

			if (!system->deinit())
			{
				Logger->error("System: {} deinit failed", system->getClassName());
			}
		}
		systems.clear();

		renderingThread.clearSystems();
		mainThread.clearSystems();

		Instance = nullptr;
	}

	EngineContext::~EngineContext() noexcept
	{
		if (Instance)
		{
			Logger->error("EngineContext must be manually deinitialized");
		}
	}

	EngineThread& EngineContext::getThreadByID(ThreadID threadID)
	{
		switch (threadID)
		{
			case ThreadID::Main:
				return mainThread;
			case ThreadID::RenderingThread:
				return renderingThread;
			default:
				Logger->error("ThreadID is out of range, returning main thread");
				return mainThread;
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