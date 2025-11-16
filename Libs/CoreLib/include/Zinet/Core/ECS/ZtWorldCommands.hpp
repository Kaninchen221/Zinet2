#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{
	class ZINET_CORE_API WorldCommands
	{
	public:

		// The World param is a placeholder so it could be used in systems
		WorldCommands(World&) {}
		WorldCommands(const WorldCommands& other) noexcept = default;
		WorldCommands(WorldCommands&& other) noexcept = default;
		~WorldCommands() noexcept = default;

		WorldCommands& operator = (const WorldCommands& other) noexcept = default;
		WorldCommands& operator = (WorldCommands&& other) noexcept = default;

		void executeCommands(World& world)
		{
			for (auto& command : commands)
			{
				command(world);
			}
		}

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
		void addResource(ResourceT&& newResource)
		{
			auto command = [newResource](World& world) { world.addResource(newResource); };
			commands.push_back(command);
		}

	protected:

		using Commands = std::vector<std::function<void(World&)>>;
		Commands commands;

	};
}