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
		//windowEvents.pollEvents();

		renderingThread.runAsync();
		mainThread.runSync();
	}

	bool EngineContext::isLooping() const
	{
		bool renderingThreadRunning = renderingThread.isRunning();
		bool mainThreadRunning = mainThread.isRunning();
		bool isWindowOpen = window.isOpen();

		return renderingThreadRunning && mainThreadRunning && isWindowOpen;
	}

	void EngineContext::stopLooping()
	{
		renderingThread.stop();
		mainThread.stop();

		renderingThread.wait();
		mainThread.wait();

		renderingThread.clearSystems();
		mainThread.clearSystems();

		window.requestCloseWindow();
	}

	void EngineContext::deinit()
	{
		if (!initialized)
			return;

		// TODO: waitCompleteJobs or wait for engine threads?
		for (auto& system : systems)
		{
			system->waitCompleteJobs();
		}

		assetsStorage.unloadAssets();

		stopLooping();

		destroyNodes(rootNode);

		for (auto& system : systems)
		{
			if (!system->deinit())
			{
				Logger->error("System: {} deinit failed", system->getClassName());
			}
		}
		systems.clear();

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