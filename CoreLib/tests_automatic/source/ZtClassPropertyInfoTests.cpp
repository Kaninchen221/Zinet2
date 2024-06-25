#pragma once

#include "Zinet/Core/Reflection/ZtClassPropertyInfo.hpp"

#include "ZtReflectionClassForClassInfo.hpp"

#include <gtest/gtest.h>
#include <xutility>

namespace zt::core::reflection::tests
{
	class ClassPropertiesInfosTests : public ::testing::Test
	{
	protected:

		const std::size_t size = 3u;

		static_assert(std::is_default_constructible_v<ClassPropertiesInfos>);
		static_assert(std::is_constructible_v<ClassPropertiesInfos, std::vector<ClassPropertyInfo>>);
		static_assert(std::is_copy_constructible_v<ClassPropertiesInfos>);
		static_assert(std::is_copy_assignable_v<ClassPropertiesInfos>);
		static_assert(std::is_move_constructible_v<ClassPropertiesInfos>);
		static_assert(std::is_move_assignable_v<ClassPropertiesInfos>);
		static_assert(std::is_destructible_v<ClassPropertiesInfos>);

		TestReflectionClassForClassInfo testClass;
		std::unique_ptr<ClassInfo> classInfo;


		void SetUp() override
		{
			classInfo = testClass.getClassInfoObject();
			ASSERT_TRUE(classInfo);
		}

		void TearDown() override
		{
		}
	};

	TEST_F(ClassPropertiesInfosTests, FindFirstWithPropertyName)
	{
 		auto properties = classInfo->getClassPropertiesInfos();
 		const std::string_view expectedPropertyName = "i1";
 		auto optClassPropertyInfo = properties.findFirstWithPropertyName(expectedPropertyName);
 		ASSERT_TRUE(optClassPropertyInfo);
 		ASSERT_EQ(optClassPropertyInfo->getPropertyName(), expectedPropertyName);
	}

	TEST_F(ClassPropertiesInfosTests, ArrowOperator)
	{
		auto properties = classInfo->getClassPropertiesInfos();
		std::vector<ClassPropertyInfo>* vector = properties.operator->();
 		ASSERT_NE(vector, nullptr);
	}

	class ClassPropertyInfoTests : public ::testing::Test
	{
	protected:

		static_assert(std::is_default_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_constructible_v<ClassPropertyInfo, std::int64_t, std::string_view, std::string_view>);
		static_assert(std::is_copy_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_copy_assignable_v<ClassPropertyInfo>);
		static_assert(std::is_move_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_move_assignable_v<ClassPropertyInfo>);
		static_assert(std::is_destructible_v<ClassPropertyInfo>);

		const size_t memoryOffset = 24;
		const std::string_view propertyName = "TestName";
		const std::string_view propertyTypeName = "TestPropertyTypeName";
		ClassPropertyInfo classPropertyInfo{ memoryOffset, propertyName, propertyTypeName };

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(ClassPropertyInfoTests, Offset)
	{
		size_t actualOffset = classPropertyInfo.getMemoryOffset();
		EXPECT_EQ(actualOffset, memoryOffset);
	}

	TEST_F(ClassPropertyInfoTests, PropertyName)
	{
		std::string_view actualPropertyName = classPropertyInfo.getPropertyName();
		EXPECT_EQ(actualPropertyName, propertyName);
	}

	TEST_F(ClassPropertyInfoTests, PropertyTypeName)
	{
		std::string_view actualPropertyTypeName = classPropertyInfo.getPropertyTypeName();
		EXPECT_EQ(actualPropertyTypeName, propertyTypeName);
	}

	class ClassPropertyInfoRealInstanceTests : public ::testing::Test
	{
	protected:

		static_assert(std::is_default_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_constructible_v<ClassPropertyInfo, std::int64_t, std::string_view, std::string_view>);
		static_assert(std::is_copy_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_copy_assignable_v<ClassPropertyInfo>);
		static_assert(std::is_move_constructible_v<ClassPropertyInfo>);
		static_assert(std::is_move_assignable_v<ClassPropertyInfo>);
		static_assert(std::is_destructible_v<ClassPropertyInfo>);

		TestReflectionClassForClassInfo testClass;

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(ClassPropertyInfoRealInstanceTests, GetClassPropertiesInfos)
	{
		auto classInfo = testClass.getClassInfoObject();
		ASSERT_TRUE(classInfo);

		const ClassPropertiesInfos classPropertiesInfos = classInfo->getClassPropertiesInfos();
 		ASSERT_FALSE(classPropertiesInfos.get().empty());
	}

	TEST_F(ClassPropertyInfoRealInstanceTests, GetCopyOfAllMembers)
	{
		auto copyOfAllMembers = testClass.getCopyOfAllMembers();
		EXPECT_EQ(testClass.i1, std::get<0>(copyOfAllMembers));
		EXPECT_EQ(testClass.b1, std::get<1>(copyOfAllMembers));
		EXPECT_EQ(testClass.someStruct, std::get<2>(copyOfAllMembers));
		EXPECT_EQ(testClass.lli1, std::get<3>(copyOfAllMembers));
		EXPECT_EQ(testClass.d1, std::get<4>(copyOfAllMembers));
		EXPECT_EQ(testClass.i2, std::get<5>(copyOfAllMembers));
	}

	TEST_F(ClassPropertyInfoRealInstanceTests, Is)
	{
		auto classInfo = testClass.getClassInfoObject();
		ASSERT_TRUE(classInfo);

		const auto members = classInfo->getClassPropertiesInfos();
		const auto& memberI1 = members.get().front();
		const bool isBool = memberI1.is<bool>();
		ASSERT_FALSE(isBool);

		const bool isInt = memberI1.is<int>();
		ASSERT_TRUE(isInt);

		const auto& memberSomeStruct = members.findFirstWithPropertyName("someStruct");
		ASSERT_TRUE(memberSomeStruct);

		const bool isSomeStruct = memberSomeStruct->is<SomeStruct>();
		ASSERT_TRUE(isSomeStruct);
	}

	TEST_F(ClassPropertyInfoRealInstanceTests, Cast)
	{
		auto classInfo = testClass.getClassInfoObject();
		ASSERT_TRUE(classInfo);

		auto classPropertiesInfos = classInfo->getClassPropertiesInfos();
		auto& classPropertyInfoI1 = classPropertiesInfos.get().front();
		const bool isI1Int = classPropertyInfoI1.is<int>();
		ASSERT_TRUE(isI1Int);

		/// Test memoryOffset is equal to 0
		ASSERT_EQ(classPropertyInfoI1.getMemoryOffset(), 0u);
		int& i1 = classPropertyInfoI1.cast<int>(&testClass);
		ASSERT_EQ(&i1, &testClass.i1);
		ASSERT_EQ(i1, testClass.i1);

		auto optClassPropertyInfo = classPropertiesInfos.findFirstWithPropertyName("i2");
		ASSERT_TRUE(optClassPropertyInfo);

		auto& classPropertyInfoI2 = *optClassPropertyInfo;
		const bool isI2Int = classPropertyInfoI2.is<int>();
		ASSERT_TRUE(isI2Int);
		
		/// Test memoryOffset is greater than 0
		ASSERT_GT(classPropertyInfoI2.getMemoryOffset(), 0u);
		int& i2 = classPropertyInfoI2.cast<int>(&testClass);
		ASSERT_EQ(&i2, &testClass.i2);
		ASSERT_EQ(i2, testClass.i2);
	}
}