#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{
	const SimpleCallbackSink::CallbackT& SimpleCallbackSink::GetCallback() noexcept
	{
		return Callback;
	}

	void SimpleCallbackSink::SetCallback(CallbackT newCallback) noexcept
	{
		Callback = newCallback;
	}

	bool SimpleCallbackSink::GetIgnoreLog() noexcept
	{
		return IgnoreLog;
	}

	void SimpleCallbackSink::SetIgnoreLog(bool value) noexcept
	{
		IgnoreLog = value;
	}

	void SimpleCallbackSink::sink_it_(const spdlog::details::log_msg& msg)
	{
		if (GetIgnoreLog())
			return;

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

}