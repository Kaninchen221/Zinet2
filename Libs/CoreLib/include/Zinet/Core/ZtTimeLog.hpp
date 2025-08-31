#pragma once

#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtLogger.hpp"

// Define ZINET_TIME_TRACE as 1 to turn it on in the release config

#if ZINET_TIME_TRACE 

namespace
{
	inline static auto TimeCheckLogger = zt::core::ConsoleLogger::Create("zt::core::anonim::ZT_TIME_CHECK");
}

#define ZT_TIME_LOG_NAME(NAME, EXPR) \
{ \
	zt::core::Clock timeLogNameClock; \
	timeLogNameClock.start(); \
	EXPR; \
	auto seconds = timeLogNameClock.getElapsedTime().getAsMilliseconds(); \
	TimeCheckLogger->info("Expression {} took: {} ms", #NAME, seconds); \
}

#define ZT_TIME_LOG(EXPR) \
{ \
	zt::core::Clock timeLogClock; \
	timeLogClock.start(); \
	EXPR; \
	auto seconds = timeLogClock.getElapsedTime().getAsMilliseconds(); \
	TimeCheckLogger->info("Expression {} took: {} ms", #EXPR, seconds); \
}

#else // ZINET_TIME_TRACE

#define ZT_TIME_LOG_NAME(NAME, EXPR) EXPR

#define ZT_TIME_LOG(EXPR) EXPR

#endif // ZINET_TIME_TRACE