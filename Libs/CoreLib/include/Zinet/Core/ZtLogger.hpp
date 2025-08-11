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

		CustomSink() = default;
		CustomSink(const CustomSink& other) = delete;
		CustomSink(CustomSink&& other) = delete;

		CustomSink& operator = (const CustomSink& other) = delete;
		CustomSink& operator = (CustomSink&& other) = delete;

		~CustomSink() noexcept = default;

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

	class ZINET_CORE_API ConsoleLogger : public Logger
	{
	public:

		inline static ConsoleLogger Create(std::string name);

	protected:

		ConsoleLogger() noexcept = default;
		ConsoleLogger(const ConsoleLogger& other) = default;
		ConsoleLogger(ConsoleLogger&& other) noexcept { *this = std::move(other); }
	
	public:

		~ConsoleLogger() noexcept = default;

		ConsoleLogger& operator = (const ConsoleLogger& other) = default;
		ConsoleLogger& operator = (ConsoleLogger&& other) noexcept = default;

		spdlog::logger* operator -> () { return impl.internal.get(); }
		const spdlog::logger* operator -> () const { return impl.internal.get(); }

		spdlog::logger* operator ()() { return impl.internal.get(); }
		const spdlog::logger* operator ()() const { return impl.internal.get(); }

		operator bool() { return impl.internal.operator bool(); }
		operator bool() const { return impl.internal.operator bool(); }

		void turnOff();
		void turnOn();

	protected:

		struct Impl
		{
			std::shared_ptr<spdlog::logger> internal;
			spdlog::level::level_enum lastLevel = spdlog::level::info;
		};
		Impl impl;

	};

	ConsoleLogger ConsoleLogger::Create(std::string name)
	{
		ConsoleLogger logger;
		logger.impl.internal = spdlog::get(name);
		if (!logger)
			logger.impl.internal = spdlog::stdout_color_mt(name);

		auto sink = std::make_shared<CustomSink>();
		auto& sinks = logger.impl.internal->sinks();
		sinks.push_back(sink);
		
		return logger;

	}
}