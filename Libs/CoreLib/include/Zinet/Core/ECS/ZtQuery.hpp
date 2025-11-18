#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>
#include <tuple>

namespace zt::core::ecs
{
	template<class IsConstType, class... Components>
	class QueryIteratorImpl
	{
	public:

		using IsConstT = IsConstType;
		using Archetypes = std::conditional_t<IsConstT{}, std::vector<const Archetype*>, std::vector<Archetype*>>;

		QueryIteratorImpl(std::vector<Archetype*> archetypes, size_t currentArchetypeIndex, size_t currentEntityIndex)
			: archetypes{ archetypes },
			currentArchetypeIndex{ currentArchetypeIndex }, 
			currentEntityIndex{ currentEntityIndex }
		{}

		QueryIteratorImpl(std::vector<const Archetype*> archetypes, size_t currentArchetypeIndex, size_t currentEntityIndex)
			: archetypes{ archetypes },
			currentArchetypeIndex{ currentArchetypeIndex },
			currentEntityIndex{ currentEntityIndex }
		{
		}

		bool operator == (const QueryIteratorImpl& other) const noexcept;
		bool operator != (const QueryIteratorImpl& other) const noexcept { return !operator==(other); }

		QueryIteratorImpl& operator++ () noexcept;

		auto operator* () const noexcept 
		{ 
			auto archetype = archetypes[currentArchetypeIndex];
			auto& entities = archetype->getEntities();

			auto entity = entities[currentEntityIndex];

			using ReturnT = std::conditional_t < IsConstT{}, std::tuple<const Components*...>, std::tuple<Components*... >> ;
			return ReturnT{ archetype->getComponentOfType<Components>(entity.getComponentsIndex())... };
		}

	private:
		
		Archetypes archetypes;
		size_t currentArchetypeIndex = 0;
		size_t currentEntityIndex = 0;

	};

	template<class IsConstType, class... Components>
	bool QueryIteratorImpl<IsConstType, Components...>::operator==(const QueryIteratorImpl& other) const noexcept
	{
		return archetypes == other.archetypes &&
			currentArchetypeIndex == other.currentArchetypeIndex &&
			currentEntityIndex == other.currentEntityIndex;
	}

	template<class IsConstType, class... Components>
	QueryIteratorImpl<IsConstType, Components...>& QueryIteratorImpl<IsConstType, Components...>::operator++() noexcept
	{
		do
		{
			if (currentArchetypeIndex >= archetypes.size())
			{
				currentArchetypeIndex = InvalidIndex;
				currentEntityIndex = InvalidIndex;
				return *this;
			}

			auto* archetype = archetypes[currentArchetypeIndex];

			++currentEntityIndex;
			if (currentEntityIndex >= archetype->getEntitiesCount())
			{
				currentArchetypeIndex++;
				currentEntityIndex = 0;

				if (currentArchetypeIndex >= archetypes.size())
				{
					currentArchetypeIndex = InvalidIndex;
					currentEntityIndex = InvalidIndex;
					return *this;
				}

				archetype = archetypes[currentArchetypeIndex];
				if (currentEntityIndex < archetype->getEntitiesCount())
					return *this;

				continue;
			}

			return *this;
		}
		while (true);

		return *this;
	}

	template<class... Components>
	using QueryIterator = QueryIteratorImpl<std::false_type, Components...>;

	template<class... Components>
	using ConstQueryIterator = QueryIteratorImpl<std::true_type, Components...>;

	// TODO: Allow to get access to each 'buffer' so the user could easily copy the data to other location
	// Example: Copying data to uniform buffers
	template<class IsConstType, class... Components>
	class QueryImpl
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::Query");

		QueryImpl() noexcept = default;

	public:

		using IsQueryType = std::true_type;
		using IsConstT = IsConstType;
		using ComponentsT = std::tuple<Components...>;

		using Archetypes = std::conditional_t<IsConstT{}, std::vector<const Archetype*>, std::vector<Archetype*>>;

		template<class WorldT>
		QueryImpl(WorldT& world)
		{
			static_assert(
				std::is_same_v<WorldT, World> ||
				std::is_same_v<WorldT, const World>,
				"You must pass the World class as a param");

			if constexpr (IsConstType{})
			{
				const World& constWorld = world;
				archetypes = constWorld.getArchetypesWith<Components...>();
			}
			else
			{
				archetypes = world.getArchetypesWith<Components...>();
			}
		}

		QueryImpl(const QueryImpl& other) noexcept = default;
		QueryImpl(QueryImpl&& other) noexcept = default;
		~QueryImpl() noexcept = default;

		QueryImpl& operator = (const QueryImpl& other) noexcept = default;
		QueryImpl& operator = (QueryImpl&& other) noexcept = default;

		size_t getComponentsCount() const noexcept;

		QueryIteratorImpl<IsConstT, Components...> begin() noexcept { return beginImpl(); }
		QueryIteratorImpl<IsConstT, Components...> begin() const noexcept { return beginImpl(); }

		QueryIteratorImpl<IsConstT, Components...> end() noexcept { return endImpl(); }
		QueryIteratorImpl<IsConstT, Components...> end() const noexcept { return endImpl(); }

	private:

		auto beginImpl() const noexcept { return archetypes.empty() ? end() : QueryIteratorImpl<IsConstT, Components...>{ archetypes, 0, 0 }; }
		auto endImpl() const noexcept { return QueryIteratorImpl<IsConstT, Components...>{ archetypes, InvalidIndex, InvalidIndex }; }

		Archetypes archetypes;

	};

	template<class IsConstType, class... Components>
	size_t QueryImpl<IsConstType, Components...>::getComponentsCount() const noexcept
	{
		size_t count = 0;
		for (const auto& archetype : archetypes)
		{
#		if ZINET_SANITY_CHECK
			if (!archetype)
			{
				Logger->critical("Query has an invalid archetype!");
				continue;
			}
#		endif

			((count += archetype->getComponentsOfType<Components>()->getObjectsCount()), ...);
		}
		return count;
	}

	template<class... Components>
	using Query = QueryImpl<std::false_type, Components...>;

	template<class... Components>
	using ConstQuery = QueryImpl<std::true_type, Components...>;
}