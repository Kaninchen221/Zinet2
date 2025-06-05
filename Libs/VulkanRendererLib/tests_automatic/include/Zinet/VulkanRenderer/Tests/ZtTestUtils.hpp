#pragma once

#include <type_traits>

#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

namespace zt::vulkan_renderer::tests
{

	template<class ClassT, class ObjectHandleT, class ...ConstructorArgsTs>
	static constexpr bool VulkanObjectDecoratorStaticTest()
	{
		if constexpr (!std::is_base_of_v<VulkanObject<ObjectHandleT>, ClassT>) return false;

		if constexpr (!std::is_constructible_v<ClassT, ObjectHandleT, ConstructorArgsTs...>) return false;
		if constexpr (std::is_default_constructible_v<ClassT>) return false;
		if constexpr (std::is_copy_constructible_v<ClassT>) return false;
		if constexpr (std::is_copy_assignable_v<ClassT>) return false;
		if constexpr (!std::is_move_constructible_v<ClassT>) return false;
		if constexpr (!std::is_move_assignable_v<ClassT>) return false;
		if constexpr (!std::is_destructible_v<ClassT>) return false;

		return true;
	}

}