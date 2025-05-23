#include "Zinet/Window/ZtGLFW.hpp"

namespace zt::wd
{
    bool GLFW::Init(bool hideWindow)
    {
        if (Initialized)
            return true;

#if ZINET_WINDOWS
		if (glfwPlatformSupported(GLFW_PLATFORM_WIN32))
		{
			glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);
			Logger->info("WIN32 is a supported platform");
		}
		else
		{
			Logger->error("WIN32 is not a supported platform while we are using windows");
			return false;
		}
#elif ZINET_LINUX
		std::string_view xdg_env_var_name = "XDG_RUNTIME_DIR";
		if (const char* env_var = std::getenv(xdg_env_var_name.data()))
		{
			Logger->info("{}: {}", xdg_env_var_name, env_var);
		}
		else
		{
			Logger->error("Couldn't get env var: {}", xdg_env_var_name);
			return false;
		}

		if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND))
		{
			glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
			Logger->info("Wayland is a supported platform");
		}
		else
		{
			Logger->error("Wayland is not a supported platform while we are in linux");
			return false;
		}
#endif

        if (glfwInit())
        {
            Logger->info("Succesfull initialize GLFW");

            glfwSetErrorCallback(GLFW::ErrorCallback);

			// Vulkan window hints
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

	bool GLFW::IsInitialized()
	{
		return Initialized;
	}

    void GLFW::HideWindow()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    void GLFW::UnhideWindow()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	}

	bool GLFW::IsVulkanSupported()
	{
		return glfwVulkanSupported() == GLFW_TRUE;
	}

	void GLFW::ErrorCallback(int errorCode, const char* errorDescription)
	{
        Logger->error("GLFW Error. Code: {}, Description: {}", errorCode, errorDescription);
	}

}