#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
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
		Entity spawn([[maybe_unused]] const Components&... components)
		{
			++lastID;

			Entity entity{ lastID };
			return entity;
		}

		template<class ...Components>
		auto getComponents()
		{
			return 0;
		}

	private:

		ID lastID = -1;

		//std::vector<Archetype> archetypes;

	};
}