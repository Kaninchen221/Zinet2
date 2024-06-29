#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/Reflection/ZtClassInfo.hpp"
#include "Zinet/Core/Reflection/ZtClassesInfos.hpp"

#include <chrono>

namespace zt::core
{

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Clock 
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
		Clock() = default;
		Clock(const Clock& other) = default;
		Clock(Clock&& other) = default;
		
		~Clock() noexcept = default;
		
		Clock& operator = (const Clock& other) = default;
		Clock& operator = (Clock&& other) = default;
		
		class ClassInfo : public zt::core::reflection::ClassInfo
		{
		public:
		
			std::string_view getClassName() const override { return "Clock"; }
			zt::core::reflection::ClassPropertiesInfos getClassPropertiesInfos() override {return zt::core::reflection::ClassPropertiesInfos(std::vector<zt::core::reflection::ClassPropertyInfo>{}); };
		
		};
		const inline static auto AddClassInfoResult = []()
		{
			auto& classesInfos = zt::core::reflection::ClassesInfos::Get();
			classesInfos.addClassInfo<ClassInfo>();
			return true;
		}();
		std::unique_ptr<zt::core::reflection::ClassInfo> getClassInfoObject() const { return std::make_unique<ClassInfo>(); }
		auto getCopyOfAllMembers() { return std::make_tuple(); };
		
/*GENERATED_CODE_END*/
	};

}