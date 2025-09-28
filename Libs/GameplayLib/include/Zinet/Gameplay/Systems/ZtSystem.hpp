#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class EngineContext;

	// TODO: Maybe even the systems should be nodes?
	// So we can have a system tree like with nodes
	// It will be more consistent
	// And then we don't need to create node as a bridge between systems
	class System : public core::Object
	{

	public:

		using Nodes = std::vector<ObjectWeakHandle<Node>>;

		System() = default;
		System(const System& other) = default;
		System(System&& other) noexcept = default;
		~System() noexcept = default;

		System& operator = (const System& other) = default;
		System& operator = (System&& other) noexcept = default;

		virtual bool init() { initialized = true; return true; }

		bool isInitialized() const noexcept { return initialized; }

		virtual void waitCompleteJobs() {}

		virtual bool deinit() { initialized = false; return true; }

		virtual void update() {}

		// TODO: Every system should implement its own addNode without overriding to avoid casting
		// But this function should invoke the system specific add node method to allow us to add nodes generically
		virtual void addNode(const ObjectWeakHandle<Node>& node) { nodes.push_back(node); }

		virtual Nodes& getNodes() noexcept { return nodes; }
		virtual const Nodes& getNodes() const noexcept { return nodes; }

		virtual void show();

	protected:

		Nodes nodes;
		bool initialized = false;

	};

}