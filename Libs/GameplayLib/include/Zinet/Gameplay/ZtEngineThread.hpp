#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	enum class ThreadID
	{
		Main = 0,
		RenderingThread = 1,
		Max = 2
	};

	class EngineThread : public core::Object
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EngineContext");

	public:

		using Systems = std::vector<ObjectHandle<System>>;
		using SystemsPerUpdatePhase = std::array<Systems, static_cast<size_t>(UpdatePhase::Max)>;

		EngineThread() = delete;
		EngineThread(const std::string& displayName, ThreadID newThreadID);
		EngineThread(const EngineThread& other) = default;
		EngineThread(EngineThread&& other) noexcept = default;
		~EngineThread() noexcept = default;

		EngineThread& operator = (const EngineThread& other) = default;
		EngineThread& operator = (EngineThread&& other) noexcept = default;

		void runAsync();

		void runSync();

		void stop() { running.store(false); }
		bool isRunning() const { return running.load(); }

		template<std::derived_from<System> SystemT>
		ObjectHandle<SystemT> addSystem(const std::string_view& displayName, UpdatePhase updatePhase = UpdatePhase::Main);

		template<std::derived_from<System> SystemT>
		ObjectHandle<SystemT> getSystem();

		ThreadID getID() const noexcept { return threadID; }

	protected:

		void runInternal();

		std::jthread thread;
		std::atomic_bool running{ false };
		ThreadID threadID{ ThreadID::Max };

		Systems systems;
		SystemsPerUpdatePhase systemsPerUpdatePhase;
	};
}

namespace zt::gameplay
{
	template<std::derived_from<System> SystemT>
	ObjectHandle<SystemT> EngineThread::addSystem(const std::string_view& displayName, UpdatePhase updatePhase)
	{
		if (updatePhase >= UpdatePhase::Max)
		{
			Logger->error("UpdatePhase is out of range, skip adding system with display name: {}", displayName);
			return {};
		}

		auto system = CreateObject<SystemT>(displayName);

		systemsPerUpdatePhase[static_cast<size_t>(updatePhase)].emplace_back(system);

		return systems.emplace_back(system);
	}

	template<std::derived_from<System> SystemT>
	ObjectHandle<SystemT> EngineThread::getSystem()
	{
		// TODO: Handle multiple systems of the same type or derived type or not? 
		// Else: assert that there is only one system of the type or derived type during addSystem
		for (auto& system : systems)
		{
			auto ptr = dynamic_cast<SystemT*>(system.get());
			if (ptr)
				return system;
		}

		return ObjectHandle<SystemT>{};
	}
}