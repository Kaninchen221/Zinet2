#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

namespace zt::core::ecs
{
	// TODO: Remove SystemReturnState?
	// Systems shouldn't return anything and instead if they want to return some error info then they should modify ExitReason?
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