#pragma once

#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ClassDefaultObjectRegistryTests : public ::testing::Test
	{
	protected:

		ClassDefaultObjectRegistryTests()
		{
		}

		~ClassDefaultObjectRegistryTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(ClassDefaultObjectRegistryTests, GetTest)
	{
		const ClassDefaultObjectRegistry& CDORegistryFirst = ClassDefaultObjectRegistry::Get();
		const ClassDefaultObjectRegistry& CDORegistrySecond = ClassDefaultObjectRegistry::Get();
		
		EXPECT_EQ(&CDORegistryFirst, &CDORegistrySecond);
	}

	TEST_F(ClassDefaultObjectRegistryTests, RegisterClassAndGetClassesTest)
	{
		ClassDefaultObjectRegistry& CDORegistry = ClassDefaultObjectRegistry::Get();
		ObjectBase* object = new Object();
		CDORegistry.registerClass(object);

		const ClassDefaultObjectRegistry& ConstCDORegistry = ClassDefaultObjectRegistry::Get();
		const auto& classes = ConstCDORegistry.getClasses();
		ASSERT_FALSE(classes.empty());
		ASSERT_EQ(&*classes.back(), object);
	}

	TEST_F(ClassDefaultObjectRegistryTests, CreateObjectByClassNameTest)
	{
		const ClassDefaultObjectRegistry& CDORegistry = ClassDefaultObjectRegistry::Get();
		const std::string className = "File";
		std::unique_ptr<ObjectBase> createdObject = CDORegistry.createObjectByClassName(className);

		ASSERT_NE(createdObject, nullptr);
	}
}