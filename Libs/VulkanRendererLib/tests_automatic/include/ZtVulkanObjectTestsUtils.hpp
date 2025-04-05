#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer::tests
{
	namespace VulkanObjectTestsUtils
	{
		template<class VulkanObjectT>
		void TestCreationDestruction(VulkanObjectT& vulkanObject, std::invocable auto createFunc, std::invocable auto destroyFunc)
		{
			const auto invalidObjectHandle = vulkanObject.get();
			ASSERT_FALSE(invalidObjectHandle);
			ASSERT_FALSE(vulkanObject.isValid());

			ASSERT_TRUE(std::invoke(createFunc));

			const auto validObjectHandle = vulkanObject.get();
			ASSERT_TRUE(validObjectHandle);
			ASSERT_TRUE(vulkanObject.isValid());

			std::invoke(destroyFunc);
			const auto invalidObjectHandleAfterDestroy = vulkanObject.get();
			ASSERT_FALSE(invalidObjectHandleAfterDestroy);
		}
	}
}