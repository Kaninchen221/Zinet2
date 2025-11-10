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
		std::vector<GraphNode> Schedule::buildGraphNodes() const
		{
			std::vector<GraphNode> result;

			// Create graph nodes from systems infos
			for (auto& system : systems)
			{
				GraphNode graphNode
				{
					.typeID = system.label,
					.after = system.after,
					.before = system.before
				};

				for (auto& otherSystem : systems)
				{
					if (otherSystem.label == graphNode.typeID)
						continue;

					if (std::ranges::contains(otherSystem.after, graphNode.typeID))
						graphNode.before.emplace_back(otherSystem.label);

					if (std::ranges::contains(otherSystem.before, graphNode.typeID))
						graphNode.after.emplace_back(otherSystem.label);
				}

				result.push_back(graphNode);
			}

			return result;
		}
	}
}