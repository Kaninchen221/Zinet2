#pragma once

#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtPointersUtilities.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::core::tests
{

	class ObjectTests : public ::testing::Test
	{
	protected:

		ObjectTests()
		{
		}

		~ObjectTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		static_assert(std::is_base_of_v<ObjectBase, Object>);

		class ObjectChild : public Object
		{
		public:
			std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<Object> result = createCopyInternal<ObjectChild>(); *result = *this; return result; }

			int value = 0;

		};
	};

	TEST_F(ObjectTests, RegisterClassTest)
	{
		const bool result = Object::RegisterClass<ObjectChild>();
		ASSERT_TRUE(result);

		ClassDefaultObjectRegistry& CDORegistry = ClassDefaultObjectRegistry::Get();
		ASSERT_FALSE(CDORegistry.getClasses().empty());
		ASSERT_NE(dynamic_cast<ObjectChild*>(CDORegistry.getClasses().back().get()), nullptr);
	}

	TEST_F(ObjectTests, GetClassInfoTest)
	{
		const Object object;
		const ClassInfoBase* classInfoFirst = object.getClassInfo();
		const ClassInfoBase* classInfoSecond = object.getClassInfo();

		ASSERT_EQ(classInfoFirst, classInfoSecond);
	}

	TEST_F(ObjectTests, CreateCopyTest)
	{
		ObjectChild object;
		object.value = 12333;
		std::unique_ptr<ObjectBase> copy = object.createCopy();
		ASSERT_NE(copy, nullptr);
		
		const ObjectBase* copyRawPointer = copy.get();
		std::unique_ptr<ObjectChild> copyCasted = PointersUtilities::DynamicCastUniquePtr<ObjectBase, ObjectChild>(copy);
		ASSERT_NE(copyCasted, nullptr);
		ASSERT_EQ(copyCasted.get(), copyRawPointer);

		ASSERT_EQ(object.value, copyCasted->value);
	}
}