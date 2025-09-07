#pragma once

#include "Zinet/Gameplay/ZtEngineThread.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace zt::gameplay::tests
{
	class EngineThreadTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::tests::EngineThreadTests");

		class TestSystem : public System
		{
			void update() override
			{
				updated.store(true);
				ASSERT_EQ(EngineThread::GetCurrentThreadID(), expectedThreadID);
			}

		public:
			std::atomic_bool updated{ false };
			ThreadID expectedThreadID = ThreadID::Max;
		};
	};

	TEST_F(EngineThreadTests, Test)
	{
		EngineThread thread_1{ "Rendering Thread", ThreadID::RenderingThread };
		ASSERT_EQ(thread_1.getDisplayName(), "Rendering Thread");
		ASSERT_EQ(thread_1.getID(), ThreadID::RenderingThread);

		EngineThread thread_2{ "Main Thread", ThreadID::Main };
		ASSERT_EQ(thread_2.getDisplayName(), "Main Thread");
		ASSERT_EQ(thread_2.getID(), ThreadID::Main);

		Logger->info("Available threads: {}", std::thread::hardware_concurrency());

		// Additional thread to turn off engine threads
		std::thread thread
		{
			[&thread_1 = thread_1, &thread_2 = thread_2]()
			{
				using namespace std::chrono_literals;

				// Wait until both threads are running
				while (!thread_1.isRunning() || !thread_1.isRunning())
				{}

				std::this_thread::sleep_for(10ms);

				thread_1.stop();
				thread_2.stop();
			}
		};

		EngineContext engineContext;
		engineContext.init();

		const auto rendererHandleFromAdd = thread_1.addSystem<TestSystem>("Renderer");
		rendererHandleFromAdd->expectedThreadID = ThreadID::RenderingThread;

		const auto tickableHandleFromAdd = thread_2.addSystem<TestSystem>("Tickable");
		tickableHandleFromAdd->expectedThreadID = ThreadID::Main;

		thread_1.runAsync();
		thread_2.runSync();

		thread.join();

		const auto rendererHandleFromGet = thread_1.getSystem<TestSystem>();
		const auto tickableHandleFromGet = thread_2.getSystem<TestSystem>();

		ASSERT_EQ(rendererHandleFromAdd, rendererHandleFromGet);
		ASSERT_EQ(tickableHandleFromAdd, tickableHandleFromGet);

		ASSERT_TRUE(rendererHandleFromAdd->updated.load());
		ASSERT_TRUE(tickableHandleFromAdd->updated.load());

		thread_1.clearSystems();
		thread_2.clearSystems();

		engineContext.deinit();
	}
}