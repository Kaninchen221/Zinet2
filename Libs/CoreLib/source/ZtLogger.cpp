#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{
	const CustomSink::CallbackT& CustomSink::GetCallback() noexcept
	{
		return Callback;
	}

	void CustomSink::SetCallback(CallbackT newCallback) noexcept
	{
		Callback = newCallback;
	}

	void CustomSink::sink_it_(const spdlog::details::log_msg& msg)
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
		impl.lastLevel = impl.internal->level();
		impl.internal->set_level(spdlog::level::off);
	}

	void ConsoleLogger::turnOn()
	{
		impl.internal->set_level(impl.lastLevel);
	}

}