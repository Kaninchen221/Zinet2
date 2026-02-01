#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClock.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"

#include <chrono>

namespace zt::gameplay::system
{
	struct DeltaTime
	{
		float value;

		operator float() const noexcept { return value; }
	};

	using DeltaTimeRes = core::ecs::Resource<DeltaTime>;
	using DeltaTimeConstRes = core::ecs::ConstResource<DeltaTime>;

	struct TickableData
	{
		core::Clock clock;
	};

	using TickableDataRes = core::ecs::Resource<TickableData>;
	using TickableDataConstRes = core::ecs::ConstResource<TickableData>;

	class ZINET_GAMEPLAY_API Tickable
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Tickable");

	public:

		static void Init(core::ecs::WorldCommands commands);

		static void Update(DeltaTimeRes deltaTimeRes, TickableDataRes tickableDataRes);

	};

}