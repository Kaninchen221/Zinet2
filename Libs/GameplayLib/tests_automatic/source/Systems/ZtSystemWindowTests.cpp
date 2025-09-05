#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemWindowTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			engineContext.addSystem<SystemWindow>("SystemWindow");

			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
	};

	TEST_F(SystemWindowTests, PassTest)
	{
		auto system = engineContext.getSystem<SystemWindow>();
		ASSERT_TRUE(system);
	}
}