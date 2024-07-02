#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtRenderer.hpp"

namespace zt::rhi::opengl
{

	void Renderer::startRenderingFrame()
	{
	}

	void Renderer::endRenderingFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

}

#endif // ZINET_USE_OPENGL