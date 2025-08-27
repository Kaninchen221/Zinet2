#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace
{
	namespace vr = zt::vulkan_renderer;
}

namespace zt::gameplay::tests
{
	class SystemImGuiTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();
			ASSERT_TRUE(window.create(2, 2));
			ASSERT_TRUE(rendererContext.create(window));
			ASSERT_TRUE(imGuiIntegration.init(rendererContext, window));
			ASSERT_TRUE(system.init());
		}

		void TearDown() override
		{
			system.deinit();
			imGuiIntegration.deinit(rendererContext);
			rendererContext.destroy();
			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		core::ObjectsStorage objectsStorage;
		vr::RendererContext rendererContext;
		wd::Window window;
		vr::ImGuiIntegration imGuiIntegration;
		SystemImGui system;
	};

	TEST_F(SystemImGuiTests, PassTest)
	{
		auto node = objectsStorage.createObject<Node>("node");
		system.addNode(node.createWeakHandle());

		system.update();
	}
}