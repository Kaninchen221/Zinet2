#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{
	// WorldCommands doesn't support non-movable classes (It's WIP, TODO)
	class ZINET_CORE_API WorldCommands
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::core::ecs::WorldCommands");

	public:

		using IsWorldCommandsType = std::true_type;

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
			auto asTuple = std::tuple(std::forward<Components>(components)...);
			using TupleT = decltype(asTuple);

			std::shared_ptr<TupleT> tupleAsSharedPtr;

			if constexpr (std::is_copy_constructible_v<TupleT>)
			{
				tupleAsSharedPtr = std::make_shared<decltype(asTuple)>(asTuple);
			}
			else if (std::is_move_constructible_v<TupleT>)
			{
				tupleAsSharedPtr = std::make_shared<decltype(asTuple)>(std::move(asTuple));
			}

			auto command = 
			[components = tupleAsSharedPtr]
			(World& world) mutable
			{
				std::apply([&](auto&&... args) 
				{
					world.spawn(std::forward<decltype(args)>(args)...);
				}, *components);
			};

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