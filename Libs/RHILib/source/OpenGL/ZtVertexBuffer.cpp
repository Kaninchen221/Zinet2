#include "Zinet/RHI/OpenGL/ZtVertexBuffer.hpp"

#ifdef ZINET_USE_OPENGL

namespace zt::rhi::opengl
{

	ZtUInt VertexBuffer::GetCurrentBoundVertexBufferID()
	{
		ZtInt boundBufferIndex = VertexBuffer::GetInvalidID();
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBufferIndex);
		return boundBufferIndex;
	}

	bool VertexBuffer::create()
	{
		if (!IsValid())
		{
			glGenBuffers(1, &ID);
			return true;
		}
		else
		{
			Logger->error("Trying to create buffer while is already created");
			return false;
		}
	}

	bool VertexBuffer::bind()
	{
		if (IsValid())
		{
			glBindBuffer(GL_ARRAY_BUFFER, ID);
			return true;
		}
		else
		{
			Logger->error("Trying to bind buffer with invalid ID");
			return false;
		}
	}

	VertexBuffer::~VertexBuffer() noexcept
	{
		release();
	}

	void VertexBuffer::copyDataFromVector(const std::vector<Vertex>& container)
	{
		const ZtSize size = sizeof(Vertex) * container.size();
		glBufferData(GL_ARRAY_BUFFER, size, container.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::release()
	{
		if (IsValid())
			glDeleteBuffers(1, &ID);
	}

	bool VertexBuffer::IsValid()
	{
		return ID != GetInvalidID();
	}

	ZtUInt VertexBuffer::getID() const
	{
		return ID;
	}

}

#endif // ZINET_USE_OPENGL