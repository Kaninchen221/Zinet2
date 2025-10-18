#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtTimeLog.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

namespace zt::gameplay::tests
{
	using namespace zt::core;

	class EngineTests : public ::testing::Test
	{
	protected:

		EngineTests() 
			: scheduleInit{ ecs::Schedule::Create(MainThread, RenderThread) },
			scheduleUpdate{ ecs::Schedule::Create(MainThread, RenderThread) },
			scheduleDeinit{ ecs::Schedule::Create(MainThread, RenderThread) }
		{
		}

		~EngineTests() override
		{
		}

		void SetUp() override
		{
			ZT_TIME_LOG(init());
		}

		void init()
		{
			scheduleInit.run(world);
			scheduleInit.requestStop();
			scheduleInit.waitForStop();
		}

		void TearDown() override
		{
			ZT_TIME_LOG(deinit());
		}

		void deinit()
		{
			scheduleDeinit.run(world);
			scheduleDeinit.requestStop();
			scheduleDeinit.waitForStop();
		}

		enum Threads : ecs::ThreadID
		{
			MainThread,
			RenderThread,
		};;

		ecs::World world;

		ecs::Schedule scheduleInit;
		ecs::Schedule scheduleUpdate;
		ecs::Schedule scheduleDeinit;

	};

	TEST_F(EngineTests, PassTest)
	{
		scheduleUpdate.run(world);
		scheduleUpdate.requestStop();
		scheduleUpdate.waitForStop();
	}
}