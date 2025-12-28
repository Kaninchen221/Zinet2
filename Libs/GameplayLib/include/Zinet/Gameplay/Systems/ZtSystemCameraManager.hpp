#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtCameraManager.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API CameraManager
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		static void Init(
			core::ecs::WorldCommands worldCommands)
		{
			worldCommands.addResource(gameplay::CameraManager{});
		}

		static void Update(
			core::ecs::Resource<gameplay::CameraManager> cameraManagerRes)
		{
			if (!cameraManagerRes)
			{
				Logger->warn("Couldn't find a camera manager res");
				return;
			}

			// TODO: I need some way to get a tick time inside systems
			cameraManagerRes->update(core::Time::FromMilliseconds(16.6f);
		}

	};
}