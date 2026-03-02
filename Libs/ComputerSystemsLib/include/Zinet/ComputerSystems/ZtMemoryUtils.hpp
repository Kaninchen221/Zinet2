#pragma once

#include <format>

namespace zt::computer_systems::memory_utils
{
	template<typename T>
	std::string ToHexString(const T& object);
}

namespace zt::computer_systems::memory_utils
{
	template<typename T>
	std::string ToHexString(const T& object)
	{;
		auto pointer = reinterpret_cast<const uint8_t*>(&object);
		const char* const format = "%.2x";
		constexpr auto size = sizeof(T);
		
		std::string result;
		result.reserve(size * 2);

		char buffer[3];
		for (size_t i = 0; i < size; ++i)
		{
			auto len = sprintf_s(buffer, format, pointer[i]);
			result.append(buffer, len);
		}

		return result;
	}
}