#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"
#include "Zinet/RHI/ZtVertex.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vector>

namespace zt::rhi::opengl
{

	class ZINET_RHI_API VertexBuffer
	{
	public:

		inline static const constexpr ZtUInt GetInvalidID() { return 0u; };
		static ZtUInt GetCurrentBoundVertexBufferID();

	protected:

		inline static core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("VertexBuffer");

	public:
		VertexBuffer() = default;
		VertexBuffer(const VertexBuffer& other) = default;
		VertexBuffer(VertexBuffer&& other) = default;

		VertexBuffer& operator = (const VertexBuffer& other) = default;
		VertexBuffer& operator = (VertexBuffer&& other) = default;

		~VertexBuffer() noexcept;

		bool create();

		bool bind();

		// Buffer must be bound first
		void copyDataFromVector(const std::vector<Vertex>& container);

		void release();

		bool IsValid();

		ZtUInt getID() const;

	protected:

		ZtUInt ID{};

	};
}

#endif // ZINET_USE_OPENGL
