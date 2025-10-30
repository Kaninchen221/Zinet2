#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>
#include <typeinfo>
#include <thread>

namespace zt::core::ecs
{
	class Schedule;

	struct ZINET_CORE_API SystemPack
	{
		ID systemTypeID;
		System system;

		template<typename T>
		bool isEqual([[maybe_unused]] T&& t) const noexcept { return systemTypeID == GetTypeID<T>(); }
	};

	struct ZINET_CORE_API Thread
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::Thread");

	public:

		friend Schedule;

		Thread(ThreadID threadID) noexcept : id{ threadID } {}
		Thread(const Thread& other) noexcept { id = other.getID(); }

		void runSync(World& world) noexcept;

		void runAsync(World& world) noexcept;

		void requestStop() noexcept;

		bool isRunning() const noexcept { return running.load(); }

		ThreadID getID() const noexcept { return id; }

		auto& getSystems() const noexcept { return systems; }

	private:

		ThreadID id;
		std::vector<SystemPack> systems;
		std::jthread thread;
		std::atomic_bool running = false;
		std::atomic_bool requestedStop = false;
	};

	// TODO 1:
	// Problems to solve:
	// - handle situation when we spawn/remove entities during system execution
	// - handle situation when we add resources during system execution (we assume that resources can't be removed)
	// Solution ideas:
	// - Create some commands in schedule that will be executed at the end of update all systems or something similar
	// 
	// TODO 2:
	// Problem to solve:
	// ImGui system reads/writes to components of class Transform on the Main Thread
	// Gameplay system writes/reads components of class Transform on the Gameplay Thread
	// It will cause race condition and undefined behaviors
	// Solution ideas:
	// - Decorated world?
	// - Create a pool of available components and remove them if they are used by some other system (on different thread)
	// - Priority levels for systems or threads
	// - Choose on the thread what to do when components are locked (skip system, wait for unlock, etc.)
	// - Mutexes around components access (bad for performance)
	// 
	// TODO 3: 
	// User should be able to define dependencies between systems while adding them to the schedule
	// Schedule should generate a graph of systems dependencies and execute them in correct order and fail if it can't be created 
	class ZINET_CORE_API Schedule
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::Schedule");

		Schedule() noexcept = default;

	public:

		using Threads = std::vector<Thread>;

		template<class... ThreadsIDs>
		static Schedule Create(ThreadsIDs... threadsIDs) noexcept;

		Schedule(const Schedule& other) noexcept = default;
		Schedule(Schedule&& other) noexcept = default;
		~Schedule() noexcept = default;

		Schedule& operator = (const Schedule& other) noexcept = default;
		Schedule& operator = (Schedule&& other) noexcept = default;

		template<class Label>
		void addSystem(Label label, System system, ThreadID threadID) noexcept;

		const auto& getThreads() const noexcept { return threads; }

		void run(World& world, ThreadID mainThreadID);

		void requestStop();

		void waitForStop() noexcept;

	private:

		Threads threads;

	};

}

namespace zt::core::ecs
{
	template<class... ThreadsIDs>
	Schedule Schedule::Create(ThreadsIDs... threadsIDs) noexcept
	{
		Schedule schedule;

		(schedule.threads.emplace_back(Thread{ threadsIDs }), ...);

		return schedule;
	}

	template<class Label>
	void Schedule::addSystem([[maybe_unused]] Label label, System system, ThreadID threadID) noexcept
	{
#	if ZINET_SANITY_CHECK
		if (threadID >= threads.size())
		{
			Logger->error("Schedule::addSystem(): Invalid thread ID {}", threadID);
			return;
		}
#	endif

		auto& thread = threads[threadID];

		thread.systems.push_back(SystemPack{ GetTypeID<Label>(), system});
	}
}