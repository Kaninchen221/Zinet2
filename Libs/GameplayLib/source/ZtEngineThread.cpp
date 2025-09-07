#include "Zinet/Gameplay/ZtEngineThread.hpp"

namespace zt::gameplay
{

	EngineThread::EngineThread(const std::string& displayName, ThreadID newThreadID)
		: Object{ displayName },
		threadID{ newThreadID }
	{

	}

	void EngineThread::runAsync()
	{
		thread = std::jthread
		{
			[&engineThread = *this, threadID = threadID]()
			{
				CurrentThreadID = threadID;
				engineThread.runInternal();
			}
		};

	};

	void EngineThread::runSync()
	{
		CurrentThreadID = threadID;
		runInternal();
	}

	void EngineThread::runInternal()
	{
		running.store(true);

		while (running.load())
		{
			for (auto& phaseSystems : systemsPerUpdatePhase)
			{
				for (auto& system : phaseSystems)
				{
					system->update();
				}
			}
		}
	}

}