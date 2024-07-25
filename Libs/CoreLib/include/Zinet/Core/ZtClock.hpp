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
		const inline static bool RegisterClassResult = RegisterClass<Clock>();
		ObjectBase* createCopy() const { Object* result = createCopyInternal<Clock>(); *result = *this; return result; }
		
		Clock() = default;
		Clock(const Clock& other) = default;
		Clock(Clock&& other) = default;
		
		~Clock() noexcept = default;
		
		Clock& operator = (const Clock& other) = default;
		Clock& operator = (Clock&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Clock"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}