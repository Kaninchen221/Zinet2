#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/sinks/base_sink.h>

#include <iostream>

namespace zt::core
{
	class ZINET_CORE_API CustomSink : public spdlog::sinks::base_sink<std::mutex>
	{
		inline static std::function<void()> Callback;

	public:

		static void SetFailTestCallback(std::function<void()> newCallback)
		{
			Callback = newCallback;
		}

		static bool GetCanFailTest() { return Callback.operator bool(); }

	protected:

		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			if (GetCanFailTest())
			{
				bool shouldFail = msg.level == spdlog::level::err ||
					msg.level == spdlog::level::warn ||
					msg.level == spdlog::level::critical;

				if (shouldFail)
				{
					std::invoke(Callback);
				}
			}
		}

		void flush_() override
		{}

	};

	class ZINET_CORE_API Logger
	{
	public:

	};

	class ZINET_CORE_API ConsoleLogger : public Logger
	{
	public:

		inline static ConsoleLogger Create(std::string name);

		spdlog::logger* operator -> () { return internal.get(); }
		const spdlog::logger* operator -> () const { return internal.get(); }

		spdlog::logger* operator ()() { return internal.get(); }
		const spdlog::logger* operator ()() const { return internal.get(); }

		operator bool() { return internal.operator bool(); }
		operator bool() const { return internal.operator bool(); }

		void turnOff();
		void turnOn();

	protected:

		std::shared_ptr<spdlog::logger> internal;
		spdlog::level::level_enum lastLevel = spdlog::level::info;

	};

	ConsoleLogger ConsoleLogger::Create(std::string name)
	{
		ConsoleLogger logger;
		logger.internal = spdlog::stdout_color_mt(name);

		auto sink = std::make_shared<CustomSink>();
		auto& sinks = logger.internal->sinks();
		sinks.push_back(sink);
		
		return logger;

	}
}