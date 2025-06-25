#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Window/ZtGLFW.hpp"

#include <concepts>

namespace zt::vulkan_renderer::tests
{
	class VulkanObjectTests : public ::testing::Test
	{
	protected:

		using HandleType = int*;
		using VulkanObjectType = VulkanObject<HandleType>;
		VulkanObjectType vulkanObject{ nullptr };
		const VulkanObjectType& vulkanObjectConst = vulkanObject;

		static_assert(std::is_same_v<HandleType, VulkanObjectType::HandleType>);

		static_assert(!std::is_default_constructible_v<VulkanObjectType>);
		static_assert(std::is_constructible_v<VulkanObjectType, HandleType>);
		static_assert(!std::is_copy_constructible_v<VulkanObjectType>);
		static_assert(!std::is_copy_assignable_v<VulkanObjectType>);
		static_assert(std::is_move_constructible_v<VulkanObjectType>);
		static_assert(std::is_move_assignable_v<VulkanObjectType>);
		static_assert(std::is_destructible_v<VulkanObjectType>);
	};

	TEST_F(VulkanObjectTests, GetTest)
	{
		HandleType ptr = vulkanObject.get();
		ASSERT_EQ(ptr, nullptr);

		const HandleType cptr = vulkanObjectConst.get();
		ASSERT_EQ(cptr, nullptr);
	}

	TEST_F(VulkanObjectTests, IsValidTest)
	{
		ASSERT_FALSE(vulkanObject.isValid());
	}

	TEST(VulkanObject, IsVulkanObjectT)
	{
		using ValidVulkanObjectT = VulkanObject<int*>;
		const auto validVulkanObjectResult = IsVulkanObjectT<ValidVulkanObjectT>;
		ASSERT_TRUE(validVulkanObjectResult);

		using InvalidVulkanObjectT = int;
		const auto invalidVulkanObjectResult = IsVulkanObjectT<InvalidVulkanObjectT>;
		ASSERT_FALSE(invalidVulkanObjectResult);
	}
}