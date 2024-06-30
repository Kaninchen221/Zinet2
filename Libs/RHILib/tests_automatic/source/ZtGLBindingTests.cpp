#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
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
        wd::GLFW::Init(true);

        wd::Window window;
        window.create();

        glbinding::initialize(glfwGetProcAddress);
        
        gl::glBegin(gl::GL_TRIANGLES);
        // ...
        gl::glEnd();
        
        window.requestCloseWindow();

        wd::GLFW::Deinit();
    }

}

#endif // ZINET_USE_OPENGL