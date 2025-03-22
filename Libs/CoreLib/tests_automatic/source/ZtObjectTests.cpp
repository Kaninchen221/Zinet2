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

			bool canBeCreatedFromAsset() const override { return Object::canBeCreatedFromAsset(); }

			std::string_view getAssetExtension() const override { return Object::getAssetExtension(); }

			std::string asString() const override { return ""; };

			int value = 0;

		};
	};

	TEST_F(ObjectTests, RegisterClassTest)
	{
		ClassDefaultObjectRegistry& CDORegistry = ClassDefaultObjectRegistry::Get();

		const auto expectedRegistrySize = CDORegistry.getCDOs().size() + 1;
		const bool result = Object::RegisterClass<ObjectChild>();
		ASSERT_TRUE(result);

		const auto actualRegistrySize = CDORegistry.getCDOs().size();
		ASSERT_EQ(expectedRegistrySize, actualRegistrySize);
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

	TEST_F(ObjectTests, CanBeCreatedFromAssetTest)
	{
		ObjectChild objectChild;

		ASSERT_FALSE(objectChild.canBeCreatedFromAsset());
	}

	TEST_F(ObjectTests, GetAssetExtensionTest)
	{
		ObjectChild objectChild;

		const std::string_view extension = objectChild.getAssetExtension();
		ASSERT_TRUE(extension.empty());
	}
}