#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

namespace zt::core::ecs
{
	// TODO: Rename to ReturnState and move it to the core namespace
	class ZINET_CORE_API SystemReturnState
	{
	public:

		enum class Level
		{
			Info,
			Warn,
			Critical,
			Error,
			Fatal
		};

		SystemReturnState(Level level, const std::string& message) : level{level}, message{message} {}

		SystemReturnState() noexcept = default;
		SystemReturnState(const SystemReturnState& other) noexcept = default;
		SystemReturnState(SystemReturnState&& other) noexcept = default;
		~SystemReturnState() noexcept = default;

		SystemReturnState& operator = (const SystemReturnState& other) noexcept = default;
		SystemReturnState& operator = (SystemReturnState&& other) noexcept = default;

		auto getLevel() const noexcept { return level; }
		const auto& getMessage() const noexcept { return message; }

	private:

		Level level = Level::Info;
		std::string message;

	};

}