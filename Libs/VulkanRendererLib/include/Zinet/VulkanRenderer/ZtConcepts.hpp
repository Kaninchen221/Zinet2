#pragma once

#include <concepts>

namespace zt::vulkan_renderer
{
	class CommandBuffer;

	template<typename T>
	concept IsCommandsT = std::invocable<T, const CommandBuffer&>;

}