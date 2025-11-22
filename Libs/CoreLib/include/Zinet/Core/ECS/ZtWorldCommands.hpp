#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API WorldCommands
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::WorldCommands");

	public:

		using IsWorldCommandsType = std::true_type;

		// The World param is a placeholder so it could be used in systems
		WorldCommands(World& world) : world{ world } {}
		WorldCommands(const WorldCommands& other) noexcept = default;
		WorldCommands(WorldCommands&& other) noexcept = default;
		~WorldCommands() noexcept
		{
			world.addCommands(commands);
		}

		WorldCommands& operator = (const WorldCommands& other) noexcept = default;
		WorldCommands& operator = (WorldCommands&& other) noexcept = default;

		/// Entities & Components
		template<class... Components>
		void spawn(Components&&... components)
		{
			auto command = [components...](World& world) { world.spawn(components...); };
			commands.push_back(command);
		}

		void remove(const Entity& entity)
		{
			auto command = [entity](World& world) { world.remove(entity); };
			commands.push_back(command);
		}

		/// Resources
		// Resources are unique by type
		// User can't remove resources

		template<class ResourceT>
		void addResource(ResourceT&& resource)
		{
			using Type = std::remove_cvref_t<ResourceT>;

			auto command = [resourceAsPtr = std::make_shared<Type>(std::move(resource))](World& world)
			{
				world.addResource(std::move(*resourceAsPtr));
			};

			commands.push_back(std::move(command));
		}

	protected:

		World& world;
		World::Commands commands;

	};
}