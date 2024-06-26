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
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		
		~Asset() noexcept = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		
		class ClassInfo : public zt::core::reflection::ClassInfo
		{
		public:
		
			std::string_view getClassName() const override { return "Asset"; }
			zt::core::reflection::ClassPropertiesInfos getClassPropertiesInfos() override {return zt::core::reflection::ClassPropertiesInfos(std::vector<zt::core::reflection::ClassPropertyInfo>{zt::core::reflection::ClassPropertyInfo{offsetof(Asset, count), "count", "int"}}); };
		
		};
		const inline static auto AddClassInfoResult = []()
		{
			auto& classesInfos = zt::core::reflection::ClassesInfos::Get();
			classesInfos.addClassInfo<ClassInfo>();
			return true;
		}();
		std::unique_ptr<zt::core::reflection::ClassInfo> getClassInfoObject() const { return std::make_unique<ClassInfo>(); }
		auto getCopyOfAllMembers() { return std::make_tuple(count); };
		
/*GENERATED_CODE_END*/

	};
}