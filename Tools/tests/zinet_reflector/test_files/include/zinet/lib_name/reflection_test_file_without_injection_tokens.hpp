#pragma once

#include "zinet/lib_name/macros.hpp"
#include "Zinet/Engine/Assets/ZtAsset.h"

#include "Zinet/Core/ZtLogger.h"

ZT_REFLECT_NAMESPACE()
namespace zt::engine
{
	ZT_REFLECT_CLASS()
	class ZINET_ENGINE_API TextureAsset : public Asset
	{
		inline static auto Logger = core::ConsoleLogger::Create("TextureAsset");

	public:

	};
}