#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include <type_traits>

namespace zt::core
{
	ZT_REFLECT_CLASS(NO_TEST_BASE_CLASS_OBJECT)
	class ZINET_CORE_API Object
	{
	public:


	protected:

		inline static constexpr bool IsObjectClassInherited = true;

	private:

	public:
/*GENERATED_CODE_START*/
		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() noexcept = default;
		
		Object& operator = (const Object& other) = default;
		Object& operator = (Object&& other) = default;
		
		class ClassInfo : zt::core::ClassInfoBase
		{
		public:
		
			static std::string_view GetClassName() { return "Object"; }
		};
		
/*GENERATED_CODE_END*/
	};

}