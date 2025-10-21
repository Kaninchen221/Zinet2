#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::gameplay::system::tests
{
	using namespace zt::core;

	class WindowTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(WindowTests, Test)
	{
		ecs::World world;

		Window::Init(world);

		auto windowRes = world.getResource<wd::Window>();
		ASSERT_TRUE(windowRes);
		ASSERT_TRUE(windowRes->isOpen());

		auto windowEventsRes = world.getResource<wd::Window>();
		ASSERT_TRUE(windowEventsRes);

		Window::Update(world);

		Window::Deinit(world);
		ASSERT_FALSE(windowRes->isOpen());
	}
}