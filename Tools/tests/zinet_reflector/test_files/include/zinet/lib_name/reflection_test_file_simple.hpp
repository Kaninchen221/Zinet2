#pragma once

#include "zinet/lib_name/macros.hpp"

ZT_REFLECT_NAMESPACE()
namespace zt::engine
{
	ZT_REFLECT_CLASS()
	class ZINET_ENGINE_API Asset : public Asset
	{
		ZT_REFLECT_METHOD()
		void update() { /* ... */ }

		ZT_REFLECT_MEMBER()
		int count;

/*GENERATED_CODE_START*/
		
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Asset>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Asset>(); *result = *this; return result; }
		
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
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		
		
/*GENERATED_CODE_END*/

	};
}