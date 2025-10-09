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
		// TODO: Use some more safe way to compare types
		const std::type_info* labelTypeInfo;
		System system;

		template<typename T>
		bool isEqual([[maybe_unused]] T&& t) const noexcept { return labelTypeInfo == &typeid(T); }
	};

	struct ZINET_CORE_API Thread
	{
		friend Schedule;

		Thread(ThreadID threadID) noexcept : id{ threadID } {}

		void run(World& world) noexcept;

		void requestStop() noexcept;

		bool isRunning() const noexcept { return running; }

		ThreadID getID() const noexcept { return id; }

		auto& getSystems() const noexcept { return systems; }

	private:

		ThreadID id;
		std::vector<SystemPack> systems;
		std::jthread thread;
		bool running = false;
		bool requestedStopValue = false;
	};

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

		void run(World& world);

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

		(schedule.threads.push_back(Thread{ threadsIDs }), ...);

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

		thread.systems.push_back(SystemPack{ &typeid(Label), system });
	}
}