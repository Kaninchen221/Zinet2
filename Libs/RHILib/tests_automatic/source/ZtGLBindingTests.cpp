#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::tests
{

    class GLBindingTests : public ::testing::Test
    {
    protected:

        GLBindingTests()
        {
        }

        ~GLBindingTests() override
        {
        }

        void SetUp() override
        {
        }

        void TearDown() override
        {
        }

    };

    TEST_F(GLBindingTests, MinimalTest)
	{
        wd::GLFW glfw;

        wd::Window window;
        window.create();

        glbinding::initialize(glfwGetProcAddress);
        
        gl::glBegin(gl::GL_TRIANGLES);
        // ...
        gl::glEnd();
        
        gl::glViewport(0, 0, 200, 200);

        window.requestCloseWindow();
    }

}

#endif // ZINET_USE_OPENGL