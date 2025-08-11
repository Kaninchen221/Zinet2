#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <chrono>

namespace zt::core
{
	class Clock : public Object
	{
	public:

		using UnderlyingClock = std::chrono::system_clock;

		ZINET_CORE_API Clock() noexcept = default;
		ZINET_CORE_API Clock(const Clock& other) noexcept = default;
		ZINET_CORE_API Clock(Clock&& other) noexcept = default;
		ZINET_CORE_API ~Clock() noexcept = default;

		ZINET_CORE_API Clock& operator = (const Clock& other) noexcept = default;
		ZINET_CORE_API Clock& operator = (Clock&& other) noexcept = default;

		ZINET_CORE_API void start() noexcept;
		ZINET_CORE_API Time restart() noexcept;
		ZINET_CORE_API Time getElapsedTime() const noexcept;

	private:

		UnderlyingClock::time_point savedTimePoint;

	};

}