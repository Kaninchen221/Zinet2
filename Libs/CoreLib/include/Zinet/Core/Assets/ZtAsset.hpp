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
		const inline static bool RegisterClassResult = RegisterClass<Asset>();
		ObjectBase* createCopy() const { Object* result = createCopyInternal<Asset>(); *result = *this; return result; }
		
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		
		~Asset() noexcept = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Asset"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{Object::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}