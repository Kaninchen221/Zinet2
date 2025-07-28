#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <chrono>

namespace zt::core
{
	class ZINET_CORE_API Clock : public Object
	{

	public:

		Clock() ZINET_API_POST : Object("Clock") {}
		Clock(const Clock& other) ZINET_API_POST = default;
		Clock(Clock&& other) ZINET_API_POST = default;
		~Clock() ZINET_API_POST = default;

		Clock& operator = (const Clock& other) ZINET_API_POST = default;
		Clock& operator = (Clock&& other) ZINET_API_POST = default;

		using UnderlyingClock = std::chrono::system_clock;

		void start();
		Time restart();
		Time getElapsedTime() const;

	private:

		UnderlyingClock::time_point savedTimePoint;

	};

}