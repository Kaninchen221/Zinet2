#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{
	const CustomSink::CallbackT& CustomSink::GetCallback() ZINET_API_POST
	{
		return Callback;
	}

	void CustomSink::SetCallback(CallbackT newCallback) ZINET_API_POST
	{
		Callback = newCallback;
	}

	void CustomSink::sink_it_(const spdlog::details::log_msg& msg)  ZINET_API_POST
	{
		if (GetCallback())
		{
			bool shouldFail = msg.level == spdlog::level::err ||
				msg.level == spdlog::level::warn ||
				msg.level == spdlog::level::critical;

			if (shouldFail)
			{
				std::invoke(GetCallback());
			}
		}
	}

	void ConsoleLogger::turnOff()
	{
		lastLevel = internal->level();
		internal->set_level(spdlog::level::off);
	}

	void ConsoleLogger::turnOn()
	{
		internal->set_level(lastLevel);
	}

}