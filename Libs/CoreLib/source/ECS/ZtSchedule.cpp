#include "Zinet/Core/ECS/ZtSchedule.hpp"

namespace zt::core::ecs
{
	void Thread::runSync(World& world) noexcept
	{
		for (const auto& systemPack : systems)
		{
			systemPack.system.invoke(world);
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

}