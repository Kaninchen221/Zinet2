#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtRendererContext.hpp"

namespace zt::rhi::opengl
{
	void RendererContext::applyClearColor()
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}

}

#endif // ZINET_USE_OPENGL