#include "Zinet/RHI/OpenGL/ZtOpenGLViewport.hpp"

#ifdef ZINET_USE_OPENGL

namespace zt::rhi
{
	void OpenGLViewport::setSize(const std::pair<ZtInt, ZtInt>& newSize)
	{
		size = newSize;
	}

	const std::pair<ZtInt, ZtInt>& OpenGLViewport::getSize() const
	{
		return size;
	}

	void OpenGLViewport::setPosition(const std::pair<ZtInt, ZtInt>& newPosition)
	{
		position = newPosition;
	}

	const std::pair<ZtInt, ZtInt>& OpenGLViewport::getPosition() const
	{
		return position;
	}

	void OpenGLViewport::apply()
	{
		gl::glViewport(position.first, position.second, size.first, size.second);
	}

}

#endif // ZINET_USE_OPENGL