#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include "Zinet/SoftwareRenderer/ZtDirectXMathSingleHeader.hpp"

namespace zt::software_renderer
{

	bool SoftwareRenderer::IsAvailable() noexcept
	{
		const auto IsDirectXMathSupported = DirectX::XMVerifyCPUSupport();
		return IsDirectXMathSupported;
	}

}