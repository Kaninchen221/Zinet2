#pragma once

#include "Zinet/ZinetNativeLang/ZtZinetNativeLangConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <string_view>

namespace zt::zinet_native_lang
{
    class ZINET_ZINET_NATIVE_LANG_API Compiler
    {
    protected:
        inline static auto Logger = core::ConsoleLogger::Create("zt::zinet_native_lang::Compiler");

    public:

        using StringViewT = std::string_view;
        using StringT = std::string;

        bool compile(const StringViewT stringView);

        StringViewT getResult() const { return result; }

    protected:

        StringT result;

    };
}