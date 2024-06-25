#pragma once

#include "Zinet/Core/Reflection/ZtClassesInfos.hpp"

#include <gtest/gtest.h>

namespace zt::core::reflection::tests
{
	class ClassInfosSimpleTests : public ::testing::Test
	{
	protected:

		static_assert(std::is_default_constructible_v<ClassesInfos>);
		static_assert(!std::is_copy_constructible_v<ClassesInfos>);
		static_assert(!std::is_copy_assignable_v<ClassesInfos>);
		static_assert(!std::is_move_constructible_v<ClassesInfos>);
		static_assert(!std::is_move_assignable_v<ClassesInfos>);
		static_assert(std::is_destructible_v<ClassesInfos>);

		ClassesInfos classesInfos;

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST(ClassInfosSimpleTest, Get)
	{
		[[maybe_unused]] const ClassesInfos& classesInfos = ClassesInfos::Get();
	}

	TEST_F(ClassInfosSimpleTests, AddClassInfo)
	{
		ClassInfo classInfo;
		classesInfos.addClassInfo<ClassInfo>();
	}

	TEST_F(ClassInfosSimpleTests, GetByClassNameInvalidResult)
	{
		const std::string_view className = ".00InvalidClassName";
		auto result = classesInfos.getByClassName(className);
		ASSERT_FALSE(result);
	}

	TEST_F(ClassInfosSimpleTests, GetByClassNameValidResult)
	{
		classesInfos.addClassInfo<ClassInfo>();
		const std::string_view className = ClassInfo().getClassName();
		auto result = classesInfos.getByClassName(className);
		ASSERT_TRUE(result);
		ASSERT_EQ(result->getClassName(), className);
	}

	TEST_F(ClassInfosSimpleTests, Get)
	{
		const auto& allInfos = classesInfos.get();
		static_assert(std::ranges::forward_range<decltype(allInfos)>);
// 		for (const auto& info : allInfos)
// 		{
// 			for (const auto& propertyInfo : info->getClassPropertiesInfos().get())
// 			{
// 				std::cout << propertyInfo.getPropertyTypeName() << " " << propertyInfo.getPropertyName() << "\n";
// 			}
// 		}
	}
}