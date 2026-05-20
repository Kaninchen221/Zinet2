#pragma once

#include "Zinet/ZinetNativeLang/ZtZinetNativeLangConfig.hpp"

#include <string>

namespace zt::zinet_native_lang
{
    class ZINET_ZINET_NATIVE_LANG_API SourceCodes
    {
    public:

        using StringT = std::string_view;

        static const StringT EmptyMain()
        {
            return ""
                "function main:\n"
                "    // Empty Main"
                "";
        }
        
    };
}