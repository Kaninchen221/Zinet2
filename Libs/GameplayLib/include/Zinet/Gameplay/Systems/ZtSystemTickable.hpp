#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClock.hpp"

namespace zt::gameplay
{
	class  SystemTickable : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemTickable");

	public:

		SystemTickable() = default;
		SystemTickable(const SystemTickable& other) = default;
		SystemTickable(SystemTickable&& other) noexcept = default;
		~SystemTickable() noexcept = default;

		SystemTickable& operator = (const SystemTickable& other) = default;
		SystemTickable& operator = (SystemTickable&& other) noexcept = default;

		void onCreate(ObjectWeakHandle<Object> newSelf) override;

		void update() override;

	protected:

		core::Clock clock;

	};

}