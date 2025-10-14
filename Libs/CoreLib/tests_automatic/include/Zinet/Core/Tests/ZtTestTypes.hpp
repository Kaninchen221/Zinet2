#pragma once

#include "Zinet/Core/ZtTime.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <string>
#include <vector>

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

		inline void entryPoint([[maybe_unused]] World& world) { doSomething(); }
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

	// Components and resources can't have any logic and virtual methods
	// But they can have complex data like std::vector, std::string and etc.
	struct NotTrivialType
	{
		// So this is kinda illegal
		
		
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::tests::NotTrivialType");
		
		NotTrivialType() noexcept { Logger->info("Constructor"); }

		NotTrivialType(const NotTrivialType& other) noexcept = delete;
		//NotTrivialType(const NotTrivialType& other) noexcept { Logger->info("Copy Constructor"); *this = other; }

		NotTrivialType(NotTrivialType&& other) noexcept { Logger->info("Move Constructor"); *this = std::forward<NotTrivialType>(other); }

		NotTrivialType& operator = (const NotTrivialType& other) = delete;
		//NotTrivialType& operator = (const NotTrivialType& other) noexcept 
		//{ 
		//	Logger->info("Copy assign"); 
		//	name = other.name;
		//	data = other.data;
		//	description = other.description;
		//	return *this; 
		//}

		NotTrivialType& operator = (NotTrivialType&& other) noexcept
		{
			Logger->info("Move assign");
			name = std::move(other.name);
			data = std::move(other.data);
			description = std::move(other.description);
			return *this;
		}
		
		virtual ~NotTrivialType() noexcept { Logger->info("Virtual Destructor"); }
		

		std::string name;
		std::vector<int32_t> data;
		std::string description;
	};
}