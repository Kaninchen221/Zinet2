#include "Zinet/Core/ECS/ZtSchedule.hpp"

namespace zt::core::ecs
{
	void Thread::run(World& world) noexcept
	{
		requestedStop.store(false);
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
			}
		};
	}

	void Thread::requestStop() noexcept
	{
		requestedStop.store(true);
	}

	void Schedule::run(World& world)
	{
		for (auto& thread : threads)
		{
			thread.run(world);
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