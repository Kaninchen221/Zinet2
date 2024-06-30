#include "Zinet/RHI/ZtRHI.hpp"

#include "Zinet/Window/ZtGLFW.hpp"

namespace zt::rhi
{

	void RHI::init()
	{
		window.create();

#ifdef ZINET_USE_OPENGL
		glbinding::initialize(glfwGetProcAddress);
#endif // ZINET_USE_OPENGL
	}

	void RHI::deinit()
	{

	}

}