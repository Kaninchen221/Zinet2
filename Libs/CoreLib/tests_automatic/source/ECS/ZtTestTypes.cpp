#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs::tests
{
	namespace TestSystemIncrementer
	{
		void entryPoint(Query<Counter> counters)
		{
			for (auto [counter] : counters)
			{
				counter->value++;
			}
		}
	}
}