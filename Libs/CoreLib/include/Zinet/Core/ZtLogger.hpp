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
	class CustomSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		using CallbackT = void(*)();

	private:
		inline static CallbackT Callback;

	public:

		ZINET_CORE_API CustomSink() = default;
		ZINET_CORE_API CustomSink(const CustomSink& other) = delete;
		ZINET_CORE_API CustomSink(CustomSink&& other) = delete;

		ZINET_CORE_API CustomSink& operator = (const CustomSink& other) = delete;
		ZINET_CORE_API CustomSink& operator = (CustomSink&& other) = delete;

		ZINET_CORE_API ~CustomSink() noexcept = default;

		ZINET_CORE_API static const CallbackT& GetCallback() noexcept;

		ZINET_CORE_API static void SetCallback(CallbackT newCallback) noexcept;

	protected:

		void sink_it_(const spdlog::details::log_msg& msg) override;

		void flush_() override {}

	};

	class ZINET_CORE_API Logger
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

		ZINET_CORE_API ~ConsoleLogger() noexcept = default;

		spdlog::logger* operator -> () { return internal.get(); }
		const spdlog::logger* operator -> () const { return internal.get(); }

		spdlog::logger* operator ()() { return internal.get(); }
		const spdlog::logger* operator ()() const { return internal.get(); }

		operator bool() { return internal.operator bool(); }
		operator bool() const { return internal.operator bool(); }

		ZINET_CORE_API void turnOff();
		ZINET_CORE_API void turnOn();

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

		auto sink = std::make_shared<CustomSink>();
		auto& sinks = logger.internal->sinks();
		sinks.push_back(sink);
		
		return logger;

	}
}