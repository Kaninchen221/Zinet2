#include "Zinet/Core/ZtTime.hpp"

namespace zt::core
{

	Time::Time() noexcept
		: timeAsNanoseconds(0.f)
	{

	}

	Time::Time(NumericType nanoseconds) noexcept
		: timeAsNanoseconds(nanoseconds)
	{

	}

	Time& Time::operator=(NumericType nanoseconds) noexcept
	{
		timeAsNanoseconds = nanoseconds;
		return *this;
	}

	Time::NumericType Time::getAsNanoseconds() const noexcept
	{
		return timeAsNanoseconds;
	}

	Time::NumericType Time::getAsMicroseconds() const noexcept
	{
		return timeAsNanoseconds / NanosecondsInMicrosecond;
	}

	Time::NumericType Time::getAsMilliseconds() const noexcept
	{
		return getAsMicroseconds() / MicrosecondsInMillisecond;
	}

	Time::NumericType Time::getAsSeconds() const noexcept
	{
		return getAsMilliseconds() / MillisecondsInSecond;
	}

	Time::NumericType Time::getAsMinutes() const noexcept
	{
		return getAsSeconds() / SecondsInMinute;
	}

	Time::NumericType Time::getAsHours() const noexcept
	{
		return getAsMinutes() / MinutesInHour;
	}

	Time Time::FromNanoseconds(NumericType nanoseconds) noexcept
	{
		return Time(nanoseconds);
	}

	Time Time::FromMicroseconds(NumericType microseconds) noexcept
	{
		return FromNanoseconds(microseconds * NanosecondsInMicrosecond);
	}

	Time Time::FromMilliseconds(NumericType milliseconds) noexcept
	{
		return FromMicroseconds(milliseconds * MicrosecondsInMillisecond);
	}

	Time Time::FromSeconds(NumericType seconds) noexcept
	{
		return FromMilliseconds(seconds * MillisecondsInSecond);
	}

	Time Time::FromMinutes(NumericType minutes) noexcept
	{
		return FromSeconds(minutes * SecondsInMinute);
	}

	Time Time::FromHours(NumericType hours) noexcept
	{
		return FromMinutes(hours * SecondsInMinute);
	}

}
