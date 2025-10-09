#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"

#include <vector>
#include <typeinfo>

namespace zt::core::ecs
{
	struct ZINET_CORE_API SystemPack
	{
		const std::type_info* labelTypeInfo;
		Function<void> system;

		template<typename T>
		bool isEqual([[maybe_unused]] T&& t) const noexcept { return labelTypeInfo == &typeid(T); }
	};

	struct ZINET_CORE_API Thread
	{
		Thread(ThreadID threadID) noexcept : id{ threadID } {}

		ThreadID id;
		std::vector<SystemPack> systems;
	};

	class ZINET_CORE_API Schedule
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::Schedule");

		Schedule() noexcept = default;

	public:

		using System = Function<void>;

		template<class... Threads>
		static Schedule Create(Threads... threadsIDs) noexcept;

		Schedule(const Schedule& other) noexcept = default;
		Schedule(Schedule&& other) noexcept = default;
		~Schedule() noexcept = default;

		Schedule& operator = (const Schedule& other) noexcept = default;
		Schedule& operator = (Schedule&& other) noexcept = default;

		template<class Label>
		void addSystem(Label label, System system, ThreadID threadID) noexcept;

		const auto& getThreads() const noexcept { return threads; }

	private:

		std::vector<Thread> threads;

	};

}

namespace zt::core::ecs
{
	template<class... Threads>
	Schedule Schedule::Create(Threads... threadsIDs) noexcept
	{
		Schedule schedule;

		schedule.threads = { Thread{ threadsIDs }... };

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