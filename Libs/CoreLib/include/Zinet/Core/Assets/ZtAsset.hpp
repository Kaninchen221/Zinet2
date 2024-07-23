#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <filesystem>

namespace zt::core::assets
{
	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Asset : public Object
	{

	public:


	protected:


	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		
		~Asset() noexcept = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		
		class ClassInfo : zt::core::ClassInfoBase
		{
		public:
		
			static std::string_view GetClassName() { return "Asset"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{Object::ClassInfo{}}; }
		};
		
/*GENERATED_CODE_END*/
	};

}