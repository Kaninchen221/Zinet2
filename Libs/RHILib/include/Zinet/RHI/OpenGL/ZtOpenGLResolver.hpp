#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/ZtRHIConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::rhi::opengl
{

	class ZINET_RHI_API OpenGLResolver
	{

	protected:

		inline static core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("OpenGLResolver");

		static void PostCallback(void* ret, const char* name, GLADapiproc apiproc, int len_args, ...)
		{
			const GLenum error_code = glad_glGetError();
			if (error_code != GL_NO_ERROR) {
				Logger->error("Glad error code: {} Name: {}", error_code, name);
			}
		}

	public:

		OpenGLResolver() = delete;
		OpenGLResolver(const OpenGLResolver& other) = delete;
		OpenGLResolver(OpenGLResolver&& other) = delete;

		OpenGLResolver& operator = (const OpenGLResolver& other) = delete;
		OpenGLResolver& operator = (OpenGLResolver&& other) = delete;

		~OpenGLResolver() noexcept = delete;

		static bool Init()
		{
			int result = gladLoaderLoadGL();
			Logger->info("gladLoadGL returned version: {}", result);

			gladSetGLPostCallback(PostCallback);

			return true;
		}

	};

}

#endif // ZINET_USE_OPENGL