#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtArchetype.hpp"
#include "Zinet/Core/ZtDebug.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API World
	{
	public:

		World() noexcept = default;
		World(const World& other) noexcept = default;
		World(World&& other) noexcept = default;
		~World() noexcept = default;

		World& operator = (const World& other) noexcept = default;
		World& operator = (World&& other) noexcept = default;

		template<class ...Components>
		Entity spawn(const Components&... components);

		template<class Component>
		Component* getComponent(const Entity& entity);

	private:

		ID lastID = -1;

		std::vector<Archetype> archetypes;

	};

	template<class... Components>
	Entity World::spawn([[maybe_unused]] const Components&... components)
	{
		++lastID;

		//auto& archetype = archetypes.emplace_back(Archetype::Create<Components...>());
		//archetype.add(components...);

		Entity entity{ lastID };
		return entity;
	}

	template<class Component>
	Component* World::getComponent([[maybe_unused]] const Entity& entity)
	{
		return {};
	}

}