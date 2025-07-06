#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API GameplayLoop
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("GameplayLoop");

	public:

		GameplayLoop() noexcept = default;
		GameplayLoop(const GameplayLoop& other) noexcept = default;
		GameplayLoop(GameplayLoop&& other) noexcept = default;
		~GameplayLoop() noexcept = default;

		GameplayLoop& operator = (const GameplayLoop& other) noexcept = default;
		GameplayLoop& operator = (GameplayLoop&& other) noexcept = default;


	protected:


	};
}