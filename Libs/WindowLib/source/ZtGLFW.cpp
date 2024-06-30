#include "Zinet/Window/ZtGLFW.hpp"

#include <GLFW/glfw3.h>

namespace zt::wd
{
    bool GLFW::Init(bool hideWindow)
    {
        if (Initialized)
            return true;

        if (glfwInit())
        {
            Logger->info("Succesfull initialize GLFW");

            //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create OpenGL context
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

            if (hideWindow)
            {
                HideWindow();
            }

            Initialized = true;
            return true;
        }
        else
        {
            Logger->error("Can't initialize GLFW");
            return false;
        }
    }

    void GLFW::Deinit()
	{
        if (!Initialized)
            return;

		Logger->info("Terminate GLFW");
        glfwTerminate();
        Initialized = false;
    }

    void GLFW::HideWindow()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    void GLFW::UnhideWindow()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    }
}