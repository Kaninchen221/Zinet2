#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/SoftwareRenderer/ZtVertex.hpp"
#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::software_renderer
{
	enum class DrawMode
	{
		Triangles,
		Lines,
		Points
	};

	struct DrawData
	{
		const Vertices* vertices;
		const Indices* indices;
		RenderTarget* renderTarget;
		DrawMode drawMode = DrawMode::Triangles;

		constexpr bool IsValid() const noexcept
		{
			return vertices != nullptr && indices != nullptr && renderTarget != nullptr;
		}
	};

	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRenderer");

	public:

		static bool IsAvailable() noexcept;

		void draw(const DrawData& drawData) noexcept;

	private:

		void drawPoints(const DrawData& drawData) noexcept;

	};

}