#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemThreadQueue.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace zt::gameplay::tests
{
	class EngineContextTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		EngineContext engineContext;

		class SystemMock : public SystemThreadQueue
		{
		public:

			inline static size_t CurrentSystemUpdatedOrder = 0;
			size_t systemUpdatedOrder = 0;

			void update() override 
			{ 
				[&systemUpdatedOrder = systemUpdatedOrder]() 
				{ systemUpdatedOrder = CurrentSystemUpdatedOrder; ++CurrentSystemUpdatedOrder; }(); 
			}
		};
	};

	TEST_F(EngineContextTests, InitTest)
	{
		ASSERT_TRUE(engineContext.init());
		auto& instance = EngineContext::Get();
		ASSERT_TRUE(&instance);
		engineContext.deinit();
	}

	TEST_F(EngineContextTests, SystemTest)
	{
		ObjectHandle<System> addSystemHandle = engineContext.addSystem<System>("System");
		ASSERT_TRUE(addSystemHandle);

		ASSERT_TRUE(engineContext.init());
		ASSERT_TRUE(addSystemHandle->isInitialized());

		ObjectHandle<System> getSystemHandle = engineContext.getSystem<System>();
		ASSERT_TRUE(getSystemHandle);

		ASSERT_EQ(addSystemHandle, getSystemHandle);

		engineContext.deinit();
	}

	TEST_F(EngineContextTests, UpdatePhaseTest)
	{
		ASSERT_TRUE(engineContext.init());

		ObjectHandle<SystemMock> system_1 = engineContext.addSystem<SystemMock>("System1", UpdatePhase::Post);
		ObjectHandle<SystemMock> system_2 = engineContext.addSystem<SystemMock>("System2", UpdatePhase::Main);
		ObjectHandle<SystemMock> system_3 = engineContext.addSystem<SystemMock>("System3", UpdatePhase::Pre);

		engineContext.loop();

		ASSERT_EQ(system_1->systemUpdatedOrder, 2);
		ASSERT_EQ(system_2->systemUpdatedOrder, 1);
		ASSERT_EQ(system_3->systemUpdatedOrder, 0);

		engineContext.deinit();
	}
}