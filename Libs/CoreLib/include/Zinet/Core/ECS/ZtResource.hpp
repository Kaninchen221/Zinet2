#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{
	template<class Type>
	class ZINET_CORE_API Resource
	{
	public:

		using T = std::decay_t<Type>;
		using IsResourceType = std::true_type;

		Resource(World& world)
			: resource{ world.getResource<T>() }
		{
		}

		Resource() noexcept = delete;
		Resource(const Resource& other) noexcept = default;
		Resource(Resource&& other) noexcept = default;
		~Resource() noexcept = default;

		Resource& operator = (const Resource& other) noexcept = default;
		Resource& operator = (Resource&& other) noexcept = default;

		operator bool() const noexcept { return resource; }

		T* operator -> () noexcept { return get(); }
		T const* operator -> () const noexcept { return get(); }

		T* get() noexcept { return resource; }
		T const* get() const noexcept { return resource; }

	private:

		T* resource{};

	};
}