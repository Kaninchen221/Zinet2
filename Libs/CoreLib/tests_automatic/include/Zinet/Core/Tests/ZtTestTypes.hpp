#pragma once

#include "Zinet/Core/ZtTime.hpp"

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
	};
}