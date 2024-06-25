#include "Zinet/Core/ZtIdentifier.hpp"

namespace zt::core
{

    Identifier::Identifier(size_t identifier)
    {
        underlyingNumber = identifier;
    }

    size_t Identifier::getUnderlyingNumber() const noexcept
    {
        return underlyingNumber;
    }

    Identifier::Identifier(Identifier& other)
    {
        *this = other;
    }

    Identifier& Identifier::operator=(Identifier& other)
    {
        underlyingNumber = other.underlyingNumber;
        other.underlyingNumber = Invalid;
        return *this;
    }

    Identifier::Identifier(Identifier&& other)
    {
        *this = other;
    }

    Identifier& Identifier::operator=(Identifier&& other)
    {
        return *this;
    }

}