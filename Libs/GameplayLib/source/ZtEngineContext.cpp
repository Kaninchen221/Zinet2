#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool EngineContext::init() ZINET_API_POST
	{
		if (initialized)
			return true;

		wd::GLFW::Init(false);

		if (!window.create())
			return false;

		if (!systemRenderer.init())
			return false;

		if (!systemImGui.init())
			return false;

		if (!assetsStorage.storeAssets())
			return false;

		rootNode = CreateNode();
		if (rootNode)
			rootNode->setName("RootNode");

		instance = this;
		initialized = true;

		return true;
	}

	void EngineContext::loop() ZINET_API_POST
	{
		windowEvents.pollEvents();

		systemImGui.update();
		systemRenderer.update();
	}

	void EngineContext::deinit() ZINET_API_POST
	{
		if (!initialized)
			return;

		destroyNodes(rootNode);

		assetsStorage.clear();

		systemImGui.deinit();
		systemRenderer.deinit();

		window.destroyWindow();
		wd::GLFW::Deinit();

		instance = nullptr;
	}

	EngineContext::~EngineContext() ZINET_API_POST
	{
		if (instance)
		{
			Logger->error("EngineContext must be manually deinitialized");
		}
	}

	void EngineContext::destroyNodes(NodeHandle<> node) ZINET_API_POST
	{
		if (!node)
			return;

		for (auto child : node->getChildren())
		{
			destroyNodes(child);
		}
		node.reset();
	}

}