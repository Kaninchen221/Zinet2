#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClock.hpp"

namespace zt::gameplay
{
	class  SystemScript : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemScript");

	public:

		SystemScript() = default;
		SystemScript(const SystemScript& other) = default;
		SystemScript(SystemScript&& other) noexcept = default;
		~SystemScript() noexcept = default;

		SystemScript& operator = (const SystemScript& other) = default;
		SystemScript& operator = (SystemScript&& other) noexcept = default;

		void update() override;

	};

}