#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::tests
{
	namespace TestSystemIncrementer
	{
		void entryPoint(ecs::Query<Counter> counters)
		{
			for (auto [counter] : counters)
			{
				counter->value++;
			}
		}
	}
}