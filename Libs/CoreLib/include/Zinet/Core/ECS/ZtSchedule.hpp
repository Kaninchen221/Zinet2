#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtFunctionTraits.hpp"
#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtTypes.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>
#include <typeinfo>
#include <thread>

namespace zt::core::ecs
{
	struct ZINET_CORE_API ResourceInfo
	{
		TypeID type;
		bool isConst = false;
	};

	struct ZINET_CORE_API QueryInfo
	{
		std::vector<TypeID> types;
		bool isConst = false;
	};

	struct ZINET_CORE_API SystemInfo
	{
		TypeID label = InvalidID;
		const std::type_info* typeInfo{};
		std::function<void(World&)> systemAdapter;
		std::vector<TypeID> before;
		std::vector<TypeID> after;
		bool mainThread = false;
		std::vector<QueryInfo> queries;
		std::vector<ResourceInfo> resources;
	};

	struct ZINET_CORE_API Before
	{
		template<class... Systems>
		Before(Systems...)
		{
			(values.push_back(GetTypeID<Systems>()), ...);
		}

		std::vector<ID> values;
	};

	struct ZINET_CORE_API After
	{
		template<class... Systems>
		After(Systems...)
		{
			(values.push_back(GetTypeID<Systems>()), ...);
		}

		std::vector<ID> values;
	};

	struct ZINET_CORE_API MainThread
	{};

	struct ZINET_CORE_API GraphNode
	{
		TypeID typeID{};
		const std::type_info* typeInfo{};
		std::function<void(World&)> systemAdapter;
		std::vector<TypeID> after;
		std::vector<TypeID> before;
		bool mainThread = false;
		Time executeTime;
		std::vector<ResourceInfo> resources;
	};

	struct ZINET_CORE_API GraphEdge
	{
		TypeID from = InvalidID;
		TypeID to = InvalidID;

		auto operator <=> (const GraphEdge& other) const noexcept = default;
	};

	struct ZINET_CORE_API GraphLayer
	{
		std::vector<GraphNode> nodes;
	};

	struct ZINET_CORE_API Graph
	{
		std::vector<GraphNode> nodes;
		std::vector<GraphEdge> edges;
		std::vector<GraphLayer> layers;
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
		constexpr void addSystem(LabelT, SystemT system, Deps... deps) noexcept
		{
			SystemInfo systemInfo
			{
				.label = GetTypeID<LabelT>(),
				.typeInfo = &typeid(LabelT)
			};

			(ResolveDeps(systemInfo, deps), ...);

			ResolveSystemTraits(systemInfo, system);

			ResolveSystemAdapter(systemInfo, system);

			systems.push_back(systemInfo);
		}

		void clear() { systems.clear(); graph = Graph{}; }

		auto& getSystems() const noexcept { return systems; }

		void buildGraph();

		void resolveGraph();

		auto& getGraph() const noexcept { return graph; }

		void runOnce(World& world);

		template<class LabelT, class SystemT>
		constexpr void runOneSystemOnce(LabelT, SystemT system, World& world) noexcept
		{
			clear();
			addSystem(LabelT{}, system);
			buildGraph();
			resolveGraph();
			runOnce(world);
			clear();
		}

	private:

		bool shouldSkipNode(const GraphNode& node, const World& world) noexcept;

		template<class Dependency>
		constexpr static void ResolveDeps(SystemInfo& systemInfo, const Dependency& dependency)
		{
			if constexpr (std::is_same_v<Dependency, Before>)
			{
				systemInfo.before = dependency.values;
			}
			else if constexpr (std::is_same_v<Dependency, After>)
			{
				systemInfo.after = dependency.values;
			}
			else if constexpr (std::is_same_v<Dependency, MainThread>)
			{
				systemInfo.mainThread = true;
			}
			else
			{
				static_assert(false, "Found a dependency that can't be resolved by this function");
			}
		}

		template<class SystemT>
		constexpr static void ResolveSystemTraits(SystemInfo& systemInfo, SystemT)
		{
			using SystemTraits = FunctionTraits<SystemT>;

			static_assert(std::is_same_v<typename SystemTraits::ReturnT, void>,
				"Every system should have a return type 'void'");

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
			constexpr bool IsWorldCommandsType = requires { typename T::IsWorldCommandsType; };

			if constexpr (IsQueryType)
			{
				AddQuery<T>(systemInfo);
			}
			else if constexpr (IsResourceType)
			{
				AddResource<T>(systemInfo);
			}
			else if constexpr (IsWorldCommandsType)
			{
				// Pass
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
				.types = GetTypesIDs<typename QueryT::ComponentsT>(std::make_index_sequence<size>()),
				.isConst = QueryT::IsConstT()
			};
			systemInfo.queries.push_back(queryInfo);
		}

		template<class ResourceT>
		constexpr static void AddResource(SystemInfo& systemInfo)
		{
			ResourceInfo resourceInfo
			{
				.type = GetTypeID<typename ResourceT::T>(),
				.isConst = ResourceT::IsConstT()
			};
			systemInfo.resources.push_back(resourceInfo);
		}

		template<class ComponentsT, size_t... N>
		constexpr static std::vector<TypeID> GetTypesIDs(std::index_sequence<N...>)
		{
			std::vector<TypeID> result;
			(result.push_back(GetTypeID<typename std::tuple_element_t<N, ComponentsT>>()), ...);

			return result;
		}

		template<class SystemT>
		constexpr static void ResolveSystemAdapter(SystemInfo& systemInfo, SystemT system)
		{
			using SystemTraits = FunctionTraits<SystemT>;

			systemInfo.systemAdapter = [system = system](World& world)
			{
				auto tuple = MakeTuple<World&, SystemTraits::ArgsCount>(world);
				std::apply(system, tuple);
			};
		}

		Systems systems;
		Graph graph;

	};
}