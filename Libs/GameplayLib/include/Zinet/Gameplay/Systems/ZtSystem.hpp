#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class EngineContext;

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

		// TODO: should return bool
		virtual void deinit() { initialized = false; }

		virtual void update() {}

		virtual void addNode(const ObjectWeakHandle<Node>& node) { nodes.push_back(node); }

		virtual Nodes& getNodes() noexcept { return nodes; }
		virtual const Nodes& getNodes() const noexcept { return nodes; }

		virtual void imGui();

	protected:

		Nodes nodes;
		bool initialized = false;

	};

}