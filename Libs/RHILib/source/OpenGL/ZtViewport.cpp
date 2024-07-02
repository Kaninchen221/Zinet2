#include "Zinet/RHI/OpenGL/ZtViewport.hpp"

#ifdef ZINET_USE_OPENGL

namespace zt::rhi::opengl
{
	void Viewport::setSize(const std::pair<ZtInt, ZtInt>& newSize)
	{
		size = newSize;
	}

	const std::pair<ZtInt, ZtInt>& Viewport::getSize() const
	{
		return size;
	}

	void Viewport::setPosition(const std::pair<ZtInt, ZtInt>& newPosition)
	{
		position = newPosition;
	}

	const std::pair<ZtInt, ZtInt>& Viewport::getPosition() const
	{
		return position;
	}

	void Viewport::apply()
	{
		glViewport(position.first, position.second, size.first, size.second);
	}

}

#endif // ZINET_USE_OPENGL