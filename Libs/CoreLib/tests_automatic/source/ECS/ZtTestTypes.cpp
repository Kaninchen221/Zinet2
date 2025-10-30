#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs::tests
{
	namespace TestSystemIncrementar
	{
		SystemReturnState entryPoint(World& world)
		{
			for (auto [counter] : Query<Counter>(world))
			{
				counter->value++;
			}

			return {};
		}
	}
}