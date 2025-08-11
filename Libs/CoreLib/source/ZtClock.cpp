#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtTime.hpp"

namespace zt::core
{

	void Clock::start() noexcept
	{
		savedTimePoint = UnderlyingClock::now();
	}

	Time Clock::restart() noexcept
	{
		Time elapsedTime = getElapsedTime();
		savedTimePoint = UnderlyingClock::now();
		return elapsedTime;
	}

	Time Clock::getElapsedTime() const noexcept
	{
		using Duration = std::chrono::duration<UnderlyingClock::rep, UnderlyingClock::period>;

		thread_local UnderlyingClock::time_point actualTimePoint;
		thread_local Duration difference;
		thread_local Time::NumericType nanoseconds;
		thread_local Time time;

		actualTimePoint = UnderlyingClock::now();
		difference = actualTimePoint - savedTimePoint;
		nanoseconds = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(difference).count());
		time = nanoseconds;
		return time;
	}

}