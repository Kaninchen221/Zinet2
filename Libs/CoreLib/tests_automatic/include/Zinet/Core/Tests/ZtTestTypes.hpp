#pragma once

#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtSystemReturnState.hpp"

#include <string>
#include <vector>
#include <cstdint>

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

		inline SystemReturnState doSomething() { return {}; }

		inline SystemReturnState entryPoint([[maybe_unused]] World& world) { doSomething(); return {}; }
	}

	namespace TestSystemIncrementar
	{
		struct Label {};

		SystemReturnState entryPoint(World& world);
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
}