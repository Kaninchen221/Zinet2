#pragma once

#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtWorldCommands.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <thread>

namespace zt::core::ecs
{
	class World;
}

namespace zt::core::ecs::tests
{
	struct Sprite
	{
		int id;
		bool operator == (const Sprite& other) const noexcept { return id == other.id; }
	};

	struct Position
	{
		float x;
		float y;

		bool operator == (const Position& other) const noexcept
		{
			return x == other.x && y == other.y;
		}
	};

	struct Velocity
	{
		float x;
		float y;

		bool operator == (const Velocity& other) const noexcept
		{
			return x == other.x && y == other.y;
		}
	};

	struct Counter
	{
		size_t value = 0;
	};

	// System must be a simple function
	// System can't contain any state, data and etc.
	// System can invoke other functions
	namespace TestSystem
	{
		struct Label {}; // Empty struct works as an unique ID for the system

		inline void doSomething() {}

		inline void entryPoint([[maybe_unused]] World& world) { doSomething();}
	}

	namespace TestSystemIncrementar
	{
		struct Label {};

		void entryPoint(World& world);
	}

	// Example of a resource class
	struct ResourceTime
	{
		Time time;

		bool operator == (const ResourceTime& other) const noexcept
		{
			return time == other.time;
		}
	};

	// Component/Resource with not trivial data
	class NotTrivialType
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::tests::NotTrivialType", spdlog::level::info);
		
		inline static int32_t ObjectsCounter = 0;

	public:

		static int32_t GetObjectsCounter() noexcept { return ObjectsCounter; }
		
		NotTrivialType() noexcept 
		{ 
			Logger->trace("Constructor");
			++ObjectsCounter;
		}

		NotTrivialType(const NotTrivialType& other) noexcept = delete;

		NotTrivialType(NotTrivialType&& other) noexcept 
		{ 
			Logger->trace("Move Constructor");
			*this = std::forward<NotTrivialType>(other); 
			++ObjectsCounter;
		}

		NotTrivialType& operator = (const NotTrivialType& other) = delete;

		NotTrivialType& operator = (NotTrivialType&& other) noexcept
		{
			Logger->trace("Move assign");
			name = std::move(other.name);
			data = std::move(other.data);
			description = std::move(other.description);
			return *this;
		}
		
		virtual ~NotTrivialType() noexcept 
		{ 
			Logger->trace("Virtual Destructor");
			--ObjectsCounter;
		}

		std::string name;
		std::vector<int32_t> data;
		std::string description;
	};

	class SystemTest_1
	{};

	class SystemTest_2
	{};

	class SystemTest_3
	{};

	class SystemTest_4
	{};

	class SystemTest_5
	{};

	class SystemTest_6
	{};

	class EmptySystemTest
	{
	public:
		static void EntryPoint() {}
	};

	class SleepSystemTest
	{
	public:
		static void Sleep1ms()
		{
			auto duration = std::chrono::milliseconds{ 1 };
			std::this_thread::sleep_for(duration);
		}
	};

	class ReadWritePositionResSystemTest
	{
	public:
		static void EntryPoint(Resource<Position>) {}
	};

	class ReadOnlyPositionResSystemTest
	{
	public:
		static void EntryPoint(ConstResource<Position>) {}
	};

	class ReadWritePositionVelocitySpriteComponentsSystemTest
	{
	public:
		static void EntryPoint(Query<Position, Velocity, Sprite>) {}
	};

	class ReadOnlyPositionVelocitySpriteComponentsSystemTest
	{
	public:
		static void EntryPoint(ConstQuery<Position, Velocity, Sprite>) {}
	};

	class AddResourceSystemTest
	{
	public:
		static void AddPosition(WorldCommands worldCommands) 
		{
			worldCommands.addResource(Position{});

			 
		}
	};

	class ExpectResourceSystemTest
	{
	public:
		static void ExpectPosition(ConstResource<Position> positionRes, WorldCommands worldCommands)
		{ 
			if (!positionRes)
			{
				ExitReason exitReason
				{
					.reason = "Expected valid resource of class Position"
				};

				worldCommands.addResource(exitReason);
			}
			else
			{
				worldCommands.addResource(ExitReason{});
			}

			
		}
	};

	class NonMovableClass
	{
	public:
		explicit NonMovableClass(int value) : value{ value } {}

		NonMovableClass() noexcept = default;
		NonMovableClass(const NonMovableClass& other) noexcept = default;
		NonMovableClass(NonMovableClass&& other) noexcept = delete;

		NonMovableClass& operator = (const NonMovableClass& other) = default;
		NonMovableClass& operator = (NonMovableClass&& other) noexcept = delete;
		~NonMovableClass() noexcept = default;

		int value = 0;
	};

	class NonCopyableClass
	{
	public:
		explicit NonCopyableClass(int value) : value{ value } {}

		NonCopyableClass() noexcept = default;
		NonCopyableClass(const NonCopyableClass& other) noexcept = delete;
		NonCopyableClass(NonCopyableClass&& other) noexcept = default;

		NonCopyableClass& operator = (const NonCopyableClass& other) = delete;
		NonCopyableClass& operator = (NonCopyableClass&& other) noexcept = default;
		~NonCopyableClass() noexcept = default;

		int value = 0;
	};
}