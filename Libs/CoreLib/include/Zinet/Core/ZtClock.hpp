#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <chrono>

namespace zt::core
{

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Clock : public Object
	{

	public:

		using UnderlyingClock = std::chrono::system_clock;

		void start();
		Time restart();
		Time getElapsedTime() const;

	private:

		UnderlyingClock::time_point savedTimePoint;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		
		Clock() = default;
		Clock(const Clock& other) = default;
		Clock(Clock&& other) = default;
		~Clock() noexcept = default;
		
		Clock& operator = (const Clock& other) = default;
		Clock& operator = (Clock&& other) = default;
		
		class ClassInfo : zt::core::ClassInfoBase
		{
		public:
		
			static std::string_view GetClassName() { return "Clock"; }
		};
		
/*GENERATED_CODE_END*/
	};

}