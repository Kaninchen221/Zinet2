#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API System
	{
	public:

		System() noexcept = default;
		System(const System& other) noexcept = default;
		System(System&& other) noexcept = default;
		~System() noexcept = default;

		System& operator = (const System& other) noexcept = default;
		System& operator = (System&& other) noexcept = default;



	};

}