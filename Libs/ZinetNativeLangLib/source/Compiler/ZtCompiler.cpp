#include "Zinet/ZinetNativeLang/Compiler/ZtCompiler.hpp"

namespace zt::zinet_native_lang
{
    bool Compiler::compile([[maybe_unused]] const StringViewT stringView)
    {
        result = R"(
; flat - 32bit memory model
; stdcall - language type
.model flat, stdcall

; Includes
;INCLUDE windows.inc
INCLUDE kernel32.inc
INCLUDE user32.inc

.data

.code

main proc

invoke ExitProcess, EAX

main endp

; Marks the end of a module and sets the program entry point to main
end main
)";

        return true;
    }
}