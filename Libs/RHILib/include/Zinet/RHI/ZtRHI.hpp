#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::rhi
{

	class ZINET_RHI_API RHI
	{

	public:

		RHI() = default;
		RHI(const RHI& other) = default;
		RHI(RHI&& other) = default;

		RHI& operator = (const RHI& other) = default;
		RHI& operator = (RHI&& other) = default;

		~RHI() noexcept = default;

		void init();
		void deinit();

	protected:

		wd::GLFW glfw;
		wd::Window window;

	};

}