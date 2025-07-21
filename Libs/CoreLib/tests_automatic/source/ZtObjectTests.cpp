#pragma once

#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
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

		Object object;
	};

	TEST_F(ObjectTests, Test)
	{
		const std::string asString = object.asString();

		Archive::BufferT buffer;
		Archive archive{ &buffer };
		archive << object;
	}
}