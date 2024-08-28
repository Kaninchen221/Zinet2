#pragma once

#include <type_traits>
#include <cstdlib>
#include <functional>

namespace zt::core
{
	template<typename FirstFunctionPointer, typename SecondFunctionPointer>
	constexpr const bool IsFunctionEqual(const SecondFunctionPointer& secondFunctionPointer)
	{
		return std::is_same<FirstFunctionPointer, SecondFunctionPointer>::value;
	}

	inline constexpr const auto LambdaFree = [](void* pointer)
	{
		std::free(pointer);
	};

	template <class ReturnType, class ClassType>
	using NonConstSimpleGetter = ReturnType & (ClassType::*)();

	template <class ReturnType, class ClassType>
	using ConstSimpleGetter = const ReturnType& (ClassType::*)() const;

	template<class ReturnType, class ClassType>
	[[nodiscard]] bool TestGetters(NonConstSimpleGetter<ReturnType, ClassType> nonConstPointer, ConstSimpleGetter<ReturnType, ClassType> constPointer, ClassType& object)
	{
		auto& nonConstObject = std::invoke(nonConstPointer, object);
		auto& constObject = std::invoke(constPointer, object);

		return &nonConstObject == &constObject;
	}
}
