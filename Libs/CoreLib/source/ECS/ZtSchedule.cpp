#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include <ranges>

namespace zt::core::ecs
{
	void Thread::runSync(World& world) noexcept
	{
		using Level = SystemReturnState::Level;

		for (const auto& systemPack : systems)
		{
			auto returnState = systemPack.system.invoke(world);

			switch (returnState.getLevel())
			{
				case Level::Info:
				break;

				case Level::Warn:
					Logger->warn(returnState.getMessage());
					break;

				case Level::Error:
					Logger->error(returnState.getMessage());
					break;

				case Level::Critical:
					Logger->critical(returnState.getMessage());
					break;

				case Level::Fatal:
					Logger->critical(returnState.getMessage());
					Terminate();
					break;
			}
		}
	}

	void Thread::runAsync(World& world) noexcept
	{
		running.store(true);

		thread = std::jthread
		{
			[&self = *this, &world = world]()
			{
				while (true)
				{
					for (const auto& systemPack : self.systems)
					{
						systemPack.system.invoke(world);
					}

					if (self.requestedStop.load())
						break;
				}

				self.running.store(false);
				self.requestedStop.store(false);
			}
		};
	}

	void Thread::requestStop() noexcept
	{
		requestedStop.store(true);
	}

	void Schedule::run(World& world, ThreadID mainThreadID)
	{
		for (auto& thread : threads)
		{
			if (thread.getID() == mainThreadID)
				thread.runSync(world);
			else if (!thread.isRunning())
				thread.runAsync(world);
		}
	}

	void Schedule::requestStop()
	{
		for (auto& thread : threads)
		{
			thread.requestStop();
		}
	}

	void Schedule::waitForStop() noexcept
	{
		for (auto& thread : threads)
		{
			while (thread.isRunning())
			{
				if (thread.thread.joinable())
					thread.thread.join();
			}
		}
	}

	namespace v2
	{
		void Schedule::buildGraph()
		{
			// Create graph nodes from systems infos
			auto& graphNodes = graph.nodes;
			for (const auto& system : systems)
			{
				GraphNode graphNode
				{
					.typeID = system.label,
					.after = system.after,
					.before = system.before
				};

				for (const auto& otherSystem : systems)
				{
					if (otherSystem.label == graphNode.typeID)
						continue;

					if (std::ranges::contains(otherSystem.after, graphNode.typeID))
						graphNode.before.emplace_back(otherSystem.label);

					if (std::ranges::contains(otherSystem.before, graphNode.typeID))
						graphNode.after.emplace_back(otherSystem.label);
				}

				graphNodes.push_back(graphNode);
			}

			// Create edges
 			auto& edges = graph.edges;
 			for (const auto& system : systems)
 			{
				for (auto& before : system.before)
				{
					GraphEdge graphEdge
					{
						.from = system.label,
						.to = before
					};

					if (!std::ranges::contains(edges, graphEdge))
						edges.push_back(graphEdge);
				}

				for (auto& after : system.after)
				{
					GraphEdge graphEdge
					{
						.from = after,
						.to = system.label
					};

					if (!std::ranges::contains(edges, graphEdge))
						edges.push_back(graphEdge);
				}
 			}
		}

		void Schedule::resolveGraph()
		{
			auto& notSortedNodes = graph.nodes;
			std::vector<GraphNode> sortedNodes;
			std::vector<GraphNode> nodesWithoutIncomingEdge;
			auto& edges = graph.edges;

			auto hasIncomingEdge = [&edges = edges](const GraphNode& node) -> bool
			{
				for (const auto& edge : edges)
				{
					if (edge.to == node.typeID)
						return true;
				}

				return false;
			};

			// First find all nodes that doesn't have an incoming edge
			for (size_t i = 0; i < notSortedNodes.size(); ++i)
			{
				const auto& node = notSortedNodes[i];
				if (!hasIncomingEdge(node))
				{
					nodesWithoutIncomingEdge.push_back(node);
					notSortedNodes.erase(notSortedNodes.begin() + i);
					--i;
				}
			}

			// Kahn's algorithm
			while (!nodesWithoutIncomingEdge.empty())
			{
				auto nodeWithoutIncomingEdge = nodesWithoutIncomingEdge.back();
				nodesWithoutIncomingEdge.pop_back();

				for (size_t notSortedNodeIndex = 0; notSortedNodeIndex < notSortedNodes.size(); ++notSortedNodeIndex)
				{
					auto& notSortedNode = notSortedNodes[notSortedNodeIndex];
					for (size_t edgeIndex = 0; edgeIndex < edges.size(); ++edgeIndex)
					{
						const auto& edge = edges[edgeIndex];
						if (edge.from == nodeWithoutIncomingEdge.typeID && edge.to == notSortedNode.typeID)
						{
							edges.erase(edges.begin() + edgeIndex);
							--edgeIndex;
						}
					}

					if (!hasIncomingEdge(notSortedNode))
					{
						nodesWithoutIncomingEdge.push_back(notSortedNode);
						notSortedNodes.erase(notSortedNodes.begin() + notSortedNodeIndex);
						--notSortedNodeIndex;
					}
				}

				sortedNodes.push_back(nodeWithoutIncomingEdge);
			}

			graph.nodes = sortedNodes;
		}

	}
}