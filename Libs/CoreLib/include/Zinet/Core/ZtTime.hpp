#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <compare>

namespace zt::core
{

	class ZINET_CORE_API Time {

	public:

		using NumericType = float;

		inline static constexpr NumericType NanosecondsInMicrosecond = 1000.f;
		inline static constexpr NumericType MicrosecondsInMillisecond = 1000.f;
		inline static constexpr NumericType MillisecondsInSecond = 1000.f;
		inline static constexpr NumericType SecondsInMinute = 60.f;
		inline static constexpr NumericType MinutesInHour = 60.f;

		Time() noexcept;
		Time(const Time& other) noexcept = default;
		Time(Time&& other) noexcept = default;
		Time(NumericType nanoseconds) noexcept;

		Time& operator = (const Time& other) noexcept = default;
		Time& operator = (Time&& other) noexcept = default;
		Time& operator = (NumericType nanoseconds) noexcept;

		~Time() noexcept = default;

		NumericType getAsNanoseconds() const noexcept;
		NumericType getAsMicroseconds() const noexcept;
		NumericType getAsMilliseconds() const noexcept;
		NumericType getAsSeconds() const noexcept;
		NumericType getAsMinutes() const noexcept;
		NumericType getAsHours() const noexcept;

		static Time FromNanoseconds(NumericType nanoseconds) noexcept;
		static Time FromMicroseconds(NumericType microseconds) noexcept;
		static Time FromMilliseconds(NumericType milliseconds) noexcept;
		static Time FromSeconds(NumericType seconds) noexcept;
		static Time FromMinutes(NumericType minutes) noexcept;
		static Time FromHours(NumericType hours) noexcept;

		auto operator <=> (const Time& other) const noexcept = default;

		friend Time operator - (const Time& first, const Time& second) noexcept;
		Time& operator -= (const Time& other) noexcept;

		friend Time operator + (const Time& first, const Time& second) noexcept;
		Time& operator += (const Time& other) noexcept;

	private:

		NumericType timeAsNanoseconds = 0.f;
	};

	inline Time operator - (const Time& first, const Time& second) noexcept
	{
		return Time{ first.timeAsNanoseconds - second.timeAsNanoseconds };
	}

	inline Time& Time::operator-=(const Time& other) noexcept
	{
		timeAsNanoseconds -= other.timeAsNanoseconds;
		return *this;
	}

	inline Time operator + (const Time& first, const Time& second) noexcept
	{
		return Time{ first.timeAsNanoseconds + second.timeAsNanoseconds };
	}

	inline Time& Time::operator+=(const Time& other) noexcept
	{
		timeAsNanoseconds += other.timeAsNanoseconds;
		return *this;
	}
}