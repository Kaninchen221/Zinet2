#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#ifdef ZINET_USE_OPENGL


namespace zt::rhi::opengl
{

	class ZINET_RHI_API GLEW
	{

	protected:

		inline static core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("GLEW");

	public:

		GLEW() = delete;
		GLEW(const GLEW& other) = delete;
		GLEW(GLEW&& other) = delete;

		GLEW& operator = (const GLEW& other) = delete;
		GLEW& operator = (GLEW&& other) = delete;

		~GLEW() noexcept = delete;

		static bool Init()
		{
			GLenum initResult = glewInit();
			if (initResult != GLEW_OK)
			{
				Logger->error("glewInit return: {}, message: {}", initResult, glewGetErrorString(initResult));
				return false;
			}

			return true;
		}

	};

}

#endif // ZINET_USE_OPENGL