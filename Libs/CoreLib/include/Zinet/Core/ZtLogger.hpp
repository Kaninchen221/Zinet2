#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/sinks/base_sink.h>

#include <iostream>
#include <memory>

namespace zt::core
{
	// TODO: Rename to SimpleCallbackSink
	class SimpleCallbackSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		using CallbackT = void(*)();

	private:
		inline static CallbackT Callback;

	public:

		SimpleCallbackSink() = default;
		SimpleCallbackSink(const SimpleCallbackSink& other) = delete;
		SimpleCallbackSink(SimpleCallbackSink&& other) = delete;

		SimpleCallbackSink& operator = (const SimpleCallbackSink& other) = delete;
		SimpleCallbackSink& operator = (SimpleCallbackSink&& other) = delete;

		~SimpleCallbackSink() noexcept = default;

		static const CallbackT& GetCallback() noexcept;

		static void SetCallback(CallbackT newCallback) noexcept;

	protected:

		void sink_it_(const spdlog::details::log_msg& msg) override;

		void flush_() override {}

	};

	class Logger
	{
	public:

	};

	class ConsoleLogger : public Logger
	{
	public:

		inline static ConsoleLogger Create(std::string name);

	protected:

		ConsoleLogger() noexcept = default;
		ConsoleLogger(const ConsoleLogger& other) = default;
		ConsoleLogger(ConsoleLogger&& other) noexcept { *this = std::move(other); }

		ConsoleLogger& operator = (const ConsoleLogger& other) = default;
		ConsoleLogger& operator = (ConsoleLogger&& other) noexcept = default;
	
	public:

		~ConsoleLogger() noexcept = default;

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
		logger.internal = spdlog::get(name);
		if (!logger)
			logger.internal = spdlog::stdout_color_mt(name);

		auto sink = std::make_shared<SimpleCallbackSink>();
		auto& sinks = logger.internal->sinks();
		sinks.push_back(sink);
		
		return logger;

	}

	inline void ConsoleLogger::turnOff()
	{
		lastLevel = internal->level();
		internal->set_level(spdlog::level::off);
	}

	inline void ConsoleLogger::turnOn()
	{
		internal->set_level(lastLevel);
	}
}