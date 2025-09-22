#pragma once

#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"
#include "Zinet/Core/ZtArchive.hpp"

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
		static_assert(std::is_default_constructible_v<Object>);
		static_assert(std::is_constructible_v<Object, std::string>);

		Object object;
	};

	TEST_F(ObjectTests, PassTest)
	{
		const std::string asString = object.asString();

		Archive::BufferT buffer;
		Archive archive{ &buffer };
		archive << object;

		auto self = object.getSelf();
		ASSERT_FALSE(self.isValid());
	}
}