#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API Entity
	{
	public:

		Entity() noexcept = delete;
		Entity(ID newID) : idValue{ newID } {}
		Entity(const Entity& other) noexcept = default;
		Entity(Entity&& other) noexcept = default;
		~Entity() noexcept = default;

		Entity& operator = (const Entity& other) noexcept = default;
		Entity& operator = (Entity&& other) noexcept = default;

		ID id() { return idValue; }

	private:

		ID idValue = InvalidID;

	};

}