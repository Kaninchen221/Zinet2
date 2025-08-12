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

		 Clock() noexcept = default;
		 Clock(const Clock& other) noexcept = default;
		 Clock(Clock&& other) noexcept = default;
		 ~Clock() noexcept = default;

		 Clock& operator = (const Clock& other) noexcept = default;
		 Clock& operator = (Clock&& other) noexcept = default;

		 void start() noexcept;
		 Time restart() noexcept;
		 Time getElapsedTime() const noexcept;

	private:

		UnderlyingClock::time_point savedTimePoint;

	};

}