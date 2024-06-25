#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

namespace zt::core::reflection::tests
{
	struct SomeStruct 
	{ 
		std::int32_t int32; 

		auto operator <=>(const SomeStruct& other) const = default;
	};

	ZT_REFLECT_CLASS()
	class TestReflectionClassForClassInfo
	{
	public:

		ZT_REFLECT_MEMBER(ReadOnly)
		int i1 = std::numeric_limits<int>::max();

		ZT_REFLECT_MEMBER(ReadOnly)
		bool b1 = false;

		ZT_REFLECT_MEMBER(ReadOnly)
		SomeStruct someStruct;

		SomeStruct someStruct2;

		ZT_REFLECT_MEMBER(ReadOnly)
		long long int lli1 = std::numeric_limits<long long int>::max();

		ZT_REFLECT_MEMBER(ReadOnly)
		double d1 = 213123.12654323454;

		ZT_REFLECT_MEMBER(ReadOnly)
		int i2 = 123123123;

	public:
/*GENERATED_CODE_START*/
		TestReflectionClassForClassInfo() = default;
		TestReflectionClassForClassInfo(const TestReflectionClassForClassInfo& other) = default;
		TestReflectionClassForClassInfo(TestReflectionClassForClassInfo&& other) = default;
		
		~TestReflectionClassForClassInfo() noexcept = default;
		
		TestReflectionClassForClassInfo& operator = (const TestReflectionClassForClassInfo& other) = default;
		TestReflectionClassForClassInfo& operator = (TestReflectionClassForClassInfo&& other) = default;
		
		const int& getI1() const { return i1; }
		
		const bool& getB1() const { return b1; }
		
		const SomeStruct& getSomeStruct() const { return someStruct; }
		
		const long long& getLli1() const { return lli1; }
		
		const double& getD1() const { return d1; }
		
		const int& getI2() const { return i2; }
		
		class ClassInfo : public zt::core::reflection::ClassInfo
		{
		public:
		
			std::string_view getClassName() const override { return "TestReflectionClassForClassInfo"; }
			zt::core::reflection::ClassPropertiesInfos getClassPropertiesInfos() override {return zt::core::reflection::ClassPropertiesInfos(std::vector<zt::core::reflection::ClassPropertyInfo>{zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, i1), "i1", "int"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, b1), "b1", "bool"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, someStruct), "someStruct", "SomeStruct"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, lli1), "lli1", "long long"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, d1), "d1", "double"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TestReflectionClassForClassInfo, i2), "i2", "int"}}); };
		
		};
		const inline static auto AddClassInfoResult = []()
		{
			auto& classesInfos = zt::core::reflection::ClassesInfos::Get();
			classesInfos.addClassInfo<ClassInfo>();
			return true;
		}();
		std::unique_ptr<zt::core::reflection::ClassInfo> getClassInfoObject() const { return std::make_unique<ClassInfo>(); }
		auto getCopyOfAllMembers() { return std::make_tuple(i1, b1, someStruct, lli1, d1, i2); };
		
/*GENERATED_CODE_END*/
	};

}