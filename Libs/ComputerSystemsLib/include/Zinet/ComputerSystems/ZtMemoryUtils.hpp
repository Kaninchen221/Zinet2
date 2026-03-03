#pragma once

#include <format>

namespace zt::computer_systems::memory_utils
{
	template<typename T>
	std::string ToHexString(const T& object);

	template<std::swappable T>
	void InPlaceSwap(T& l, T& r);

	template<class T>
	void ReverseArray(T& array);
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
			auto len = sprintf_s(buffer, format, pointer[i]); // I hate this
			result.append(buffer, len);
		}

		return result;
	}

	template<std::swappable T>
	void InPlaceSwap(T& l, T& r)
	{
		// a ^ a is 0
		// 0 ^ a is a

		///////////// l                        r
		r = l ^ r; // l                        l ^ r
		l = l ^ r; // l ^ l ^ r = 0 ^ r = r    l ^ r
		r = l ^ r; // r                        r ^ l ^ r = r ^ r ^ l = 0 ^ l = l
		           // r                        l
	}

	template<class T>
	void ReverseArray(T& array)
	{
		size_t last = array.size() - 1;
		for (size_t first = 0; first < last; ++first)
		{
			InPlaceSwap(array[first], array[last]);
			--last;
		}
	}
}