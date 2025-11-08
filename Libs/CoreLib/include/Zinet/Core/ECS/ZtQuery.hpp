#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>
#include <tuple>

namespace zt::core::ecs
{
	template<class... Components>
	class QueryIteratorImpl
	{
	public:

		QueryIteratorImpl(std::vector<Archetype*> archetypes, size_t currentArchetypeIndex, size_t currentEntityIndex)
			: archetypes{ archetypes },
			currentArchetypeIndex{ currentArchetypeIndex }, 
			currentEntityIndex{ currentEntityIndex }
		{}

		bool operator == (const QueryIteratorImpl& other) const noexcept;
		bool operator != (const QueryIteratorImpl& other) const noexcept { return !operator==(other); }

		QueryIteratorImpl& operator++ () noexcept;

		const std::tuple<Components*...> operator* () const noexcept { return {}; }
		std::tuple<Components*...> operator* () noexcept 
		{ 
			auto archetype = archetypes[currentArchetypeIndex];
			auto& entities = archetype->getEntities();

			auto entity = entities[currentEntityIndex];

			return std::tuple<Components*...>{ archetype->getComponentOfType<Components>(entity.getComponentsIndex())... };
		}

	private:
		
		std::vector<Archetype*> archetypes;
		size_t currentArchetypeIndex = 0;
		size_t currentEntityIndex = 0;

	};

	template<class... Components>
	bool QueryIteratorImpl<Components...>::operator==(const QueryIteratorImpl& other) const noexcept
	{
		return archetypes == other.archetypes &&
			currentArchetypeIndex == other.currentArchetypeIndex &&
			currentEntityIndex == other.currentEntityIndex;
	}

	template<class... Components>
	QueryIteratorImpl<Components...>& QueryIteratorImpl<Components...>::operator++() noexcept
	{
		do
		{
			if (currentArchetypeIndex >= archetypes.size())
			{
				currentArchetypeIndex = InvalidIndex;
				currentEntityIndex = InvalidIndex;
				return *this;
			}

			Archetype* archetype = archetypes[currentArchetypeIndex];

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
	class QueryImpl
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::Query");

		QueryImpl() noexcept = default;

	public:

		using IsQueryType = std::true_type;
		using ComponentsT = std::tuple<Components...>;

		QueryImpl(World& world)
			: archetypes(world.getArchetypesWith<Components...>())
		{
		}

		QueryImpl(const QueryImpl & other) noexcept = default;
		QueryImpl(QueryImpl&& other) noexcept = default;
		~QueryImpl() noexcept = default;

		QueryImpl& operator = (const QueryImpl& other) noexcept = default;
		QueryImpl& operator = (QueryImpl&& other) noexcept = default;

		size_t getComponentsCount() const noexcept;

		QueryIteratorImpl<Components...> begin() noexcept { return beginImpl(); }
		QueryIteratorImpl<Components...> begin() const noexcept { return beginImpl(); }

		QueryIteratorImpl<Components...> end() noexcept { return endImpl(); }
		QueryIteratorImpl<Components...> end() const noexcept { return endImpl(); }

	private:

		auto beginImpl() const noexcept { return archetypes.empty() ? end() : QueryIteratorImpl<Components...>{ archetypes, 0, 0 }; }
		auto endImpl() const noexcept { return QueryIteratorImpl<Components...>{ archetypes, InvalidIndex, InvalidIndex }; }

		std::vector<Archetype*> archetypes;

	};

	template<class... Components>
	size_t QueryImpl<Components...>::getComponentsCount() const noexcept
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
	using Query = QueryImpl<Components...>;
}