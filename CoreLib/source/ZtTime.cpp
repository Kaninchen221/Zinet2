#include "Zinet/Core/ZtTime.hpp"

namespace zt::core
{

	Time::Time()
		: timeAsNanoseconds(0.f)
	{

	}

	Time::Time(NumericType nanoseconds)
		: timeAsNanoseconds(nanoseconds)
	{

	}

	Time& Time::operator=(NumericType nanoseconds)
	{
		timeAsNanoseconds = nanoseconds;
		return *this;
	}

	Time::NumericType Time::getAsNanoseconds() const
	{
		return timeAsNanoseconds;
	}

	Time::NumericType Time::getAsMicroseconds() const
	{
		return timeAsNanoseconds / NanosecondsInMicrosecond;
	}

	Time::NumericType Time::getAsMilliseconds() const
	{
		return getAsMicroseconds() / MicrosecondsInMillisecond;
	}

	Time::NumericType Time::getAsSeconds() const
	{
		return getAsMilliseconds() / MillisecondsInSecond;
	}

	Time::NumericType Time::getAsMinutes() const
	{
		return getAsSeconds() / SecondsInMinute;
	}

	Time::NumericType Time::getAsHours() const
	{
		return getAsMinutes() / MinutesInHour;
	}

	Time Time::FromNanoseconds(NumericType nanoseconds)
	{
		return Time(nanoseconds);
	}

	Time Time::FromMicroseconds(NumericType microseconds)
	{
		return FromNanoseconds(microseconds * NanosecondsInMicrosecond);
	}

	Time Time::FromMilliseconds(NumericType milliseconds)
	{
		return FromMicroseconds(milliseconds * MicrosecondsInMillisecond);
	}

	Time Time::FromSeconds(NumericType seconds)
	{
		return FromMilliseconds(seconds * MillisecondsInSecond);
	}

	Time Time::FromMinutes(NumericType minutes)
	{
		return FromSeconds(minutes * SecondsInMinute);
	}

	Time Time::FromHours(NumericType hours)
	{
		return FromMinutes(hours * SecondsInMinute);
	}

}
