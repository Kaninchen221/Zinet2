#pragma once

#include "Zinet/Core/ZtIDTypes.hpp"

#include "Zinet/Core/ZtTypeTraits.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class IDTests : public ::testing::Test
	{
	protected:

		ID id;

		static_assert(std::is_default_constructible_v<ID>);
		static_assert(std::is_constructible_v<ID, size_t>);
		static_assert(std::is_copy_constructible_v<ID>);
		static_assert(std::is_copy_assignable_v<ID>);
		static_assert(std::is_move_constructible_v<ID>);
		static_assert(std::is_move_assignable_v<ID>);
		static_assert(std::is_destructible_v<ID>);

		static_assert(ID::InvalidIDNumber == std::numeric_limits<size_t>::max());
	};

	TEST_F(IDTests, GetNumber)
	{
		size_t actual = id.getNumber();
		size_t expected = ID::InvalidIDNumber;
		EXPECT_EQ(actual, expected);

		expected = 5u;
		id = ID{ expected };
		actual = id.getNumber();
		EXPECT_EQ(actual, expected);
	}

	TEST_F(IDTests, CompareWithNumber)
	{
		EXPECT_FALSE(id == 5u);
		EXPECT_EQ(id, ID::InvalidIDNumber);
	}

	class UniqueIDTests : public ::testing::Test
	{
	protected:

		UniqueID uniqueID;

		static_assert(std::is_default_constructible_v<UniqueID>);
		static_assert(std::is_constructible_v<UniqueID, size_t>);
		static_assert(!std::is_copy_constructible_v<UniqueID>);
		static_assert(!std::is_copy_assignable_v<UniqueID>);
		static_assert(std::is_move_constructible_v<UniqueID>);
		static_assert(std::is_move_assignable_v<UniqueID>);
		static_assert(std::is_destructible_v<UniqueID>);

		static_assert(std::is_base_of_v<ID, UniqueID>);
	};

	TEST_F(UniqueIDTests, MoveConstructible)
	{
		const size_t expected = 1033u;
		uniqueID = UniqueID{ expected };
		UniqueID other{ std::move(uniqueID) };

		EXPECT_EQ(other.getNumber(), expected);
		EXPECT_EQ(uniqueID.getNumber(), UniqueID::InvalidIDNumber);
	}

	TEST_F(UniqueIDTests, MoveAssignable)
	{
		const size_t expected = 1033u;
		uniqueID = UniqueID{ expected };
		UniqueID other = std::move(uniqueID);

		EXPECT_EQ(other.getNumber(), expected);
		EXPECT_EQ(uniqueID.getNumber(), UniqueID::InvalidIDNumber);
	}

	TEST_F(UniqueIDTests, createID)
	{
		ID id = uniqueID.createID();

		EXPECT_EQ(id, uniqueID);
	}
}
