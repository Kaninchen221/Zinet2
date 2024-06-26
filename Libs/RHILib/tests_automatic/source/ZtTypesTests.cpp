#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::tests
{

#ifdef ZINET_USE_OPENGL
	static_assert(sizeof(ZtInt) == sizeof(gl::GLint));
	static_assert(std::is_same_v<ZtInt, gl::GLint>);
#endif // ZINET_USE_OPENGL
	
}