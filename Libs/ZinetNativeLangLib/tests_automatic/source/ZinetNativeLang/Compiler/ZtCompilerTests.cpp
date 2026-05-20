#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/ZinetNativeLang/Compiler/ZtCompiler.hpp"
#include "Zinet/ZinetNativeLang/ZtSimplePrograms.hpp"

namespace zt::zinet_native_lang::tests
{
    class ZinetNativeLangTests : public testing::TestWithParam<std::string_view>
    {
    protected:

        inline static auto Logger = core::ConsoleLogger::Create("zt::zinet_native_lang::tests::ZinetNativeLangTests");

    };

    TEST_P(ZinetNativeLangTests, SingleSourceCodesTest)
    {
        const auto param = GetParam();
        Logger->info("Compile:\n'\n{}\n'", param);

        Compiler compiler;
        ASSERT_TRUE(compiler.compile(param));

        const auto& result = compiler.getResult();
        ASSERT_FALSE(result.empty());
    }

    INSTANTIATE_TEST_SUITE_P(
        SingleSourceFile,
        ZinetNativeLangTests,
        testing::Values(SourceCodes::EmptyMain())
    );
}