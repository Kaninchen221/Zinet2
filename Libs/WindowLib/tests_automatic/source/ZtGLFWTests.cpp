#pragma once

#include "Zinet/Window/ZtGLFW.hpp"

#include <gtest/gtest.h>

namespace zt::wd
{

	class GLFWTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(GLFWTests, IsInitialized)
	{
		bool isInitialized = GLFW::IsInitialized();
		ASSERT_FALSE(isInitialized);

		GLFW glfw2;
		glfw2.Init();
		isInitialized = GLFW::IsInitialized();
		ASSERT_TRUE(isInitialized);

		isInitialized = GLFW::IsInitialized();
		ASSERT_TRUE(isInitialized);

		GLFW::Deinit();
	}

	TEST_F(GLFWTests, InitGLFWTest)
	{
		const bool result = GLFW::Init();

		ASSERT_TRUE(result);

		GLFW::Deinit();
	}

	TEST_F(GLFWTests, DeinitGLFWTest)
	{
		GLFW::Init();
		GLFW::Deinit();
	}

}
