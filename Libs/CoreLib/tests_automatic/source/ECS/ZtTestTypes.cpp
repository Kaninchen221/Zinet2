#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs::tests
{
	namespace TestSystemIncrementar
	{
		void entryPoint(World& world)
		{
			for (Counter& counter : Query<Counter>(world))
			{
				counter.value++;
			}
		}
	}
}