#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

#include <vector>

namespace zt::core::ecs
{
	template<class Component>
	class QueryIterator
	{
	public:

		QueryIterator(
			QueryTypes::ComponentsPack* newComponentsPack,
			QueryTypes::ComponentsPack::iterator newSegmentIterator,
			size_t newCurrentComponentIndex
		)
			: componentsPack(newComponentsPack)
			, currentSegmentIterator(newSegmentIterator)
			, currentComponentIndex(newCurrentComponentIndex)
		{}

		bool operator == (const QueryIterator& other) const noexcept;
		bool operator != (const QueryIterator& other) const noexcept { return !operator==(other); }

		QueryIterator& operator++ () noexcept;

		const Component& operator* () const noexcept { return *(*currentSegmentIterator)->get<Component>(currentComponentIndex); }
		Component& operator* () noexcept { return *(*currentSegmentIterator)->get<Component>(currentComponentIndex); }

	private:

		QueryTypes::ComponentsPack* componentsPack{};
		QueryTypes::ComponentsPack::iterator currentSegmentIterator;
		size_t currentComponentIndex = 0;
	};

	template<class Component>
	bool QueryIterator<Component>::operator==(const QueryIterator& other) const noexcept
	{
		return 
			componentsPack == other.componentsPack && 
			currentSegmentIterator == other.currentSegmentIterator &&
			currentComponentIndex == other.currentComponentIndex;
	}

	template<class Component>
	QueryIterator<Component>& QueryIterator<Component>::operator++() noexcept
	{
		//static auto Logger = ConsoleLogger::Create("zt::core::ecs::QueryIterator<Component>::operator++()", spdlog::level::debug);

		currentComponentIndex++;

		while (!(*currentSegmentIterator)->isValidIndex(currentComponentIndex))
		{
			if (currentComponentIndex >= (*currentSegmentIterator)->getComponentsCapacity())
			{
				//Logger->debug("component index out of segment capacity component index {}", currentComponentIndex);
				currentSegmentIterator++;
				currentComponentIndex = 0;

				if (currentSegmentIterator == componentsPack->end())
				{
					currentComponentIndex = InvalidIndex;
					//Logger->debug("end of components pack component index {}", currentComponentIndex);
					return *this;
				}
			}
			else
			{
				currentComponentIndex++;
			}
		}

		//Logger->debug("component index {}", currentComponentIndex);
		return *this;
	}

	// TODO Query for more than one type or allow to zip to queries
	// TODO Handle situation when we spawn/remove entities during iteration 
	// - handle situation when we spawn new archetypes
	template<class Component>
	class Query
	{
		Query() noexcept = default;

	public:

		Query(World& world)
			: componentsPack(world.getComponentsOfType<Component>())
		{}

		Query(const Query & other) noexcept = default;
		Query(Query && other) noexcept = default;
		~Query() noexcept = default;

		Query& operator = (const Query & other) noexcept = default;
		Query& operator = (Query && other) noexcept = default;

		size_t getComponentsCount() const noexcept;

		QueryIterator<Component> begin() noexcept;

		QueryIterator<Component> end() noexcept;

	private:

		QueryTypes::ComponentsPack componentsPack;

	};

	template<class Component>
	QueryIterator<Component> Query<Component>::begin() noexcept
	{
		for (auto segmentIt = componentsPack.begin(); segmentIt != componentsPack.end(); ++segmentIt)
		{
			auto firstValidIndex = (*segmentIt)->getFirstValidIndex();
			if (firstValidIndex != InvalidIndex)
			{
				//static auto Logger = ConsoleLogger::Create("zt::core::ecs::Query<Component>::begin", spdlog::level::debug);
				//Logger->debug("first valid index {}", firstValidIndex);

				return QueryIterator<Component>{ &componentsPack, segmentIt, firstValidIndex };
			}
		}

		return end();
	}

	template<class Component>
	QueryIterator<Component> Query<Component>::end() noexcept
	{
		return QueryIterator<Component>{ &componentsPack, componentsPack.end(), InvalidIndex };
	}

	template<class Component>
	size_t Query<Component>::getComponentsCount() const noexcept
	{
		size_t count = 0;
		for (const auto& components : componentsPack)
		{
			count += components->getComponentsCount();
		}
		return count;
	}
}