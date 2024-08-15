#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <cstdint>

namespace zt::core
{
    class ZINET_CORE_API Identifier {
    public:

        const static inline size_t Invalid = std::numeric_limits<size_t>::max();

        Identifier() = delete;
        explicit Identifier(size_t identifier);
        Identifier(Identifier& other);
        Identifier(Identifier&& other);

        Identifier& operator = (Identifier& other);
        Identifier& operator = (Identifier&& other);

        ~Identifier() = default;

        size_t getUnderlyingNumber() const noexcept;

    private:

        size_t underlyingNumber = Invalid;

    };

}