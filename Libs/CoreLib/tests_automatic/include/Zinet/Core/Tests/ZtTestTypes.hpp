#pragma once

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

}