#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtFunctionTraits.hpp"

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
	// - Systems shouldn't consume a World& but Queue<const ComponentT_1, ComponentT_2> so the scheduler can gather info which data the system will use (How to store them in the scheduler?)
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

	namespace v2
	{
		struct ZINET_CORE_API QueryInfo
		{
			std::vector<TypeID> types;
		};

		struct ZINET_CORE_API SystemInfo
		{
			TypeID label = InvalidID;
			Function<void, World&> system;
			std::vector<TypeID> before;
			std::vector<TypeID> after;
			std::vector<QueryInfo> queries;
		};

		class ZINET_CORE_API Thread
		{
			std::vector<SystemInfo> systems;
		};

		class ZINET_CORE_API Graph
		{
		public:

			auto& getThreads() const noexcept { return threads; }

		private:

			std::vector<Thread> threads;

		};

		struct ZINET_CORE_API Before
		{
			template<class... Systems>
			Before([[maybe_unused]] Systems... systems)
			{
				(values.push_back(GetTypeID<Systems>()), ...);
			}

			std::vector<ID> values;
		};

		struct ZINET_CORE_API After
		{
			template<class... Systems>
			After([[maybe_unused]] Systems... systems)
			{
				(values.push_back(GetTypeID<Systems>()), ...);
			}

			std::vector<ID> values;
		};

		class ZINET_CORE_API Schedule
		{
			inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::Schedule");

		public:

			using Systems = std::vector<SystemInfo>;

			Schedule() noexcept = default;
			Schedule(const Schedule& other) noexcept = default;
			Schedule(Schedule&& other) noexcept = default;
			~Schedule() noexcept = default;

			Schedule& operator = (const Schedule& other) noexcept = default;
			Schedule& operator = (Schedule&& other) noexcept = default;

			template<class LabelT, class SystemT, class... Deps>
			void addSystem(
				[[maybe_unused]] LabelT label,
				[[maybe_unused]] SystemT system,
				[[maybe_unused]] Deps... deps) noexcept 
			{
				SystemInfo systemInfo
				{
					.label = GetTypeID<LabelT>(),
					.system = []([[maybe_unused]] World& world) {  },
				};

				(ResolveDeps(systemInfo, deps), ...);

				ResolveSystemTraits(systemInfo, system);

				systems.push_back(systemInfo);
			}

			auto& getSystems() const noexcept { return systems; }

			SystemReturnState buildGraph() 
			{ 

				return {}; 
			}

			auto& getGraph() const noexcept { return graph; }

		private:

			template<class Dependency>
			static void ResolveDeps(SystemInfo& systemInfo, const Dependency& dependency)
			{
				if constexpr (std::is_same_v<Dependency, Before>)
				{
					systemInfo.before = dependency.values;
				}
				else if constexpr (std::is_same_v<Dependency, After>)
				{
					systemInfo.after = dependency.values;
				}
				else
				{
					static_assert(false, "Found a dependency that can't be resolved by this function");
				}
			}

			template<class SystemT>
			constexpr static void ResolveSystemTraits([[maybe_unused]] SystemInfo& systemInfo, [[maybe_unused]] SystemT system)
			{
				using SystemTraits = FunctionTraits<SystemT>;

				static_assert(std::is_same_v<typename SystemTraits::ReturnT, SystemReturnState>, 
					"Every system should return this type");

				if constexpr (SystemTraits::ArgsCount > 0)
				{
					ResolveSystemArgs<SystemTraits>(systemInfo, std::make_index_sequence<SystemTraits::ArgsCount>());
				}
			}

			template<class SystemTraitsT, size_t... N>
			constexpr static void ResolveSystemArgs(SystemInfo& systemInfo, std::index_sequence<N...>)
			{
				(ResolveSystemArgsImpl<typename SystemTraitsT::template ArgsTs<N>>(systemInfo), ...);
			}

			template<class T>
			constexpr static void ResolveSystemArgsImpl(SystemInfo& systemInfo)
			{
				constexpr bool IsQueryType = requires { typename T::IsQueryType; };
				constexpr bool IsResourceType = requires { typename T::IsResourceType; };

				if constexpr (IsQueryType)
				{
					AddQuery<T>(systemInfo);
				}
				else if (IsResourceType) // Placeholder for Resource
				{

				}
				else
				{
					static_assert(false, "System has a param that is not a type that can be handled by the Schedule");
				}
			}

			template<class QueryT>
			constexpr static void AddQuery(SystemInfo& systemInfo)
			{
				constexpr size_t size = std::tuple_size<typename QueryT::ComponentsT>{};

				QueryInfo queryInfo
				{
					.types = GetTypesIDs<typename QueryT::ComponentsT>(std::make_index_sequence<size>())
				};
				systemInfo.queries.push_back(queryInfo);
			}

			template<class ComponentsT, size_t... N>
			constexpr static std::vector<TypeID> GetTypesIDs(std::index_sequence<N...>)
			{
				std::vector<TypeID> result;
				(result.push_back(GetTypeID<typename std::tuple_element_t<N, ComponentsT>>()), ...);

				return result;
			}

			Systems systems;
			Graph graph;

		};

	}
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