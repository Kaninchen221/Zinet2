#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::software_renderer
{
	class ZINET_SOFTWARE_RENDERER_API SoftwareRenderer
	{
	public:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("SoftwareRenderer");

		SoftwareRenderer() = delete;
		SoftwareRenderer(const SoftwareRenderer& other) = delete;
		SoftwareRenderer(SoftwareRenderer&& other) = delete;

		SoftwareRenderer& operator = (const SoftwareRenderer& other) = delete;
		SoftwareRenderer& operator = (SoftwareRenderer&& other) = delete;

		~SoftwareRenderer() noexcept = delete;

		void pass();
	};
}