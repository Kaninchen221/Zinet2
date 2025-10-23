#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

using namespace zt::core;

namespace zt::gameplay::system::tests
{
	class RendererTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(RendererTests, PassTest)
	{
		ecs::World world;

		Window::Init(world);
		Renderer::Init(world);

		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
		ASSERT_TRUE(rendererRes);
		ASSERT_TRUE(rendererRes->isInitialized());

		Renderer::Update(world);
		// TODO: Test Update

		Renderer::Deinit(world);
		Window::Deinit(world);

		ASSERT_FALSE(rendererRes->isInitialized());
	}
}