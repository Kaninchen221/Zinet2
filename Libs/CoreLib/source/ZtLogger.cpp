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

	void SimpleCallbackSink::sink_it_(const spdlog::details::log_msg& msg)
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

}