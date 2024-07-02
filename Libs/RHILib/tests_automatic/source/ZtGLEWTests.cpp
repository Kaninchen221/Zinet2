#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/RHI/OpenGL/ZtGLEW.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

    class GLEWTests : public ::testing::Test
    {
    protected:

        GLEWTests()
        {
        }

        ~GLEWTests() override
        {
        }

        void SetUp() override
        {
        }

        void TearDown() override
        {
        }

    };

    TEST_F(GLEWTests, Test)
	{
        wd::GLFW glfw;
        wd::Window window;
        window.create();
        GLEW::Init();
    }

}

#endif // ZINET_USE_OPENGL