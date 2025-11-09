#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>
#include <tuple>

namespace zt::core::ecs
{
	template<bool IsConst, class... Components>
	class QueryIteratorImpl
	{
	public:

		using Archetypes = std::conditional_t<IsConst, std::vector<const Archetype*>, std::vector<Archetype*>>;

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

			using ReturnT = std::conditional_t<IsConst, std::tuple<const Components*...>, std::tuple<Components*...>>;
			return ReturnT{ archetype->getComponentOfType<Components>(entity.getComponentsIndex())... };
		}

	private:
		
		Archetypes archetypes;
		size_t currentArchetypeIndex = 0;
		size_t currentEntityIndex = 0;

	};

	template<bool IsConst, class... Components>
	bool QueryIteratorImpl<IsConst, Components...>::operator==(const QueryIteratorImpl& other) const noexcept
	{
		return archetypes == other.archetypes &&
			currentArchetypeIndex == other.currentArchetypeIndex &&
			currentEntityIndex == other.currentEntityIndex;
	}

	template<bool IsConst, class... Components>
	QueryIteratorImpl<IsConst, Components...>& QueryIteratorImpl<IsConst, Components...>::operator++() noexcept
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
	using QueryIterator = QueryIteratorImpl<false, Components...>;

	template<class... Components>
	using ConstQueryIterator = QueryIteratorImpl<true, Components...>;

	template<bool IsConst, class... Components>
	class QueryImpl
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::Query");

		QueryImpl() noexcept = default;

	public:

		using IsQueryType = std::true_type;
		using ComponentsT = std::tuple<Components...>;

		using Archetypes = std::conditional_t<IsConst, std::vector<const Archetype*>, std::vector<Archetype*>>;

		QueryImpl(World& world)
			: archetypes(world.getArchetypesWith<Components...>())
		{
		}

		QueryImpl(const World& world)
			: archetypes(world.getArchetypesWith<Components...>())
		{
		}

		QueryImpl(const QueryImpl & other) noexcept = default;
		QueryImpl(QueryImpl&& other) noexcept = default;
		~QueryImpl() noexcept = default;

		QueryImpl& operator = (const QueryImpl& other) noexcept = default;
		QueryImpl& operator = (QueryImpl&& other) noexcept = default;

		size_t getComponentsCount() const noexcept;

		QueryIteratorImpl<IsConst, Components...> begin() noexcept { return beginImpl(); }
		QueryIteratorImpl<IsConst, Components...> begin() const noexcept { return beginImpl(); }

		QueryIteratorImpl<IsConst, Components...> end() noexcept { return endImpl(); }
		QueryIteratorImpl<IsConst, Components...> end() const noexcept { return endImpl(); }

	private:

		auto beginImpl() const noexcept { return archetypes.empty() ? end() : QueryIteratorImpl<IsConst, Components...>{ archetypes, 0, 0 }; }
		auto endImpl() const noexcept { return QueryIteratorImpl<IsConst, Components...>{ archetypes, InvalidIndex, InvalidIndex }; }

		Archetypes archetypes;

	};

	template<bool IsConst, class... Components>
	size_t QueryImpl<IsConst, Components...>::getComponentsCount() const noexcept
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
	using Query = QueryImpl<false, Components...>;

	template<class... Components>
	using ConstQuery = QueryImpl<true, Components...>;
}