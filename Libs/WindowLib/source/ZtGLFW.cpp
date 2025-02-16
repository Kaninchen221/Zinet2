#include "Zinet/Window/ZtGLFW.hpp"

namespace zt::wd
{
    bool GLFW::Init(bool hideWindow)
    {
        if (Initialized)
            return true;

        if (glfwInit())
        {
            Logger->info("Succesfull initialize GLFW");

            glfwSetErrorCallback(GLFW::ErrorCallback);

#if ZINET_LINUX
            if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND))
			{
				glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
                Logger->info("Wayland is a supported platform");
            }
            else
            {
                Logger->error("Wayland is not a supported platform");
                return false;
            }
#endif


#ifdef ZINET_USE_OPENGL
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
	        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create OpenGL context
#endif // ZINET_USE_OPENGL

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

	void GLFW::ErrorCallback(int errorCode, const char* errorDescription)
	{
        Logger->error("GLFW Error. Code: {}, Description: {}", errorCode, errorDescription);
	}

}