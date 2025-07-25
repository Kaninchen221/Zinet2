#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class EngineContext;

	class ZINET_GAMEPLAY_API System : public core::Object
	{

	public:

		using Nodes = std::vector<NodeWeakHandle<Node>>;

		System() ZINET_API_POST = delete;
		System(std::string_view displayName) : Object{ displayName } {}
		System(const System& other) ZINET_API_POST = default;
		System(System&& other) ZINET_API_POST = default;
		~System() ZINET_API_POST = default;

		System& operator = (const System& other) ZINET_API_POST = default;
		System& operator = (System&& other) ZINET_API_POST = default;

		virtual bool init() ZINET_API_POST { initialized = true; return true; }

		virtual void deinit() ZINET_API_POST { initialized = false; }

		virtual void update() ZINET_API_POST {}

		virtual void addNode(const NodeWeakHandle<Node>& node) ZINET_API_POST { nodes.push_back(node); }

		virtual Nodes& getNodes() ZINET_API_POST { return nodes; }
		virtual const Nodes& getNodes() const ZINET_API_POST { return nodes; }

		virtual void imGui() ZINET_API_POST;

	protected:

		Nodes nodes;
		bool initialized = false;

	};

}