#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTypes.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API Entity
	{
	public:

		Entity() noexcept = delete;
		Entity(ID id, size_t componentsIndex)
			: id{ id }, componentsIndex{ componentsIndex }
		{}
		Entity(const Entity& other) noexcept = default;
		Entity(Entity&& other) noexcept = default;
		~Entity() noexcept = default;

		Entity& operator = (const Entity& other) noexcept = default;
		Entity& operator = (Entity&& other) noexcept = default;

		ID getID() const noexcept { return id; }

		size_t getComponentsIndex() const noexcept { return componentsIndex; }

		bool operator == (const Entity& other) const noexcept { return id == other.id && componentsIndex == other.componentsIndex; }

	private:

		ID id = InvalidID;
		size_t componentsIndex = InvalidIndex;
	};

}