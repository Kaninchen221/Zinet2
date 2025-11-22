#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Gameplay/Components/ZtRenderDrawData.hpp"

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

	TEST_F(RendererTests, Test)
	{
// 		ecs::World world;
// 
// 		Window::Init(world);
// 		Renderer::Init(world);
// 
// 		bool commandInvoked = false;
// 		auto command = [&commandInvoked = commandInvoked]([[maybe_unused]] vulkan_renderer::CommandBuffer& commandBuffer)
// 		{
// 			commandInvoked = true;
// 		};
// 		component::RenderDrawData renderDrawData{ command };
// 		world.spawn(renderDrawData);
// 
// 		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
// 		ASSERT_TRUE(rendererRes);
// 		ASSERT_TRUE(rendererRes->isInitialized());
// 
// 		Renderer::Update(world);
// 
// 		EXPECT_TRUE(commandInvoked);
// 
// 		Renderer::Deinit(world);
// 		Window::Deinit(world);
// 
// 		ASSERT_FALSE(rendererRes->isInitialized());
	}
}