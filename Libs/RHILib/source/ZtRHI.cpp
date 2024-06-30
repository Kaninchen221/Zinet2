#include "Zinet/RHI/ZtRHI.hpp"

#include "Zinet/Window/ZtGLFW.hpp"

namespace zt::rhi
{

	void RHI::init()
	{
		wd::GLFW::Init(true);

#ifdef ZINET_USE_OPENGL
		glbinding::initialize(glfwGetProcAddress);
#endif // ZINET_USE_OPENGL
	}

	void RHI::deinit()
	{
		wd::GLFW::Deinit();
	}

}