#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <imgui.h>
#include <fmt/format.h>

namespace ImGui
{
	template<typename... T>
	inline void TextFMT(fmt::format_string<T...> formatString, T&&... args)
	{
		auto str = fmt::format(formatString, std::forward<decltype(args)>(args)...);
		ImGui::TextUnformatted(str.data(), str.data() + str.size());
	}

	inline void Text(const std::string_view str)
	{
		ImGui::TextUnformatted(str.data(), str.data() + str.size());
	}

	inline void TextCStr(const char* str)
	{
		Text(std::string_view(str));
	}
}