#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"

namespace zt::core::ecs
{
	// System must be a simple function
	// System can't contain any state, data and etc.
	// System can invoke other functions
	namespace TestSystem
	{
		struct Label {}; // Empty struct works as an unique ID for the system

		inline void doSomething() {}

		inline void entryPoint() { doSomething(); }
	}

	class ECSScheduleTests : public ::testing::Test
	{
	protected:

		enum Threads : uint8_t
		{
			MainThread,
			RenderThread
		};

	};

	TEST_F(ECSScheduleTests, CreateScheduleTest)
	{
		Schedule schedule = Schedule::Create(Threads::MainThread, Threads::RenderThread);
		const auto& threads = schedule.getThreads();
		ASSERT_EQ(threads.size(), 2);

		ASSERT_EQ(threads[0].id, Threads::MainThread);
		ASSERT_EQ(threads[1].id, Threads::RenderThread);
	}

	TEST_F(ECSScheduleTests, AddSystemTest)
	{
		Schedule schedule = Schedule::Create(Threads::MainThread, Threads::RenderThread);

		schedule.addSystem(TestSystem::Label{}, TestSystem::entryPoint, Threads::MainThread);

		struct LambdaLabel {};
		auto lambda = []() { TestSystem::doSomething(); };

		schedule.addSystem(LambdaLabel{}, lambda, Threads::RenderThread);

		const auto& threads = schedule.getThreads();

		ASSERT_TRUE(threads[Threads::MainThread].systems[0].isEqual(TestSystem::Label{}));
		ASSERT_TRUE(threads[Threads::RenderThread].systems[0].isEqual(LambdaLabel{}));
	}
}