#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <type_traits>

namespace zt::core
{
	template<class ReturnType, class ...Args>
	class Function
	{
	public:

		using ThisType = Function<ReturnType, Args...>;
		using ReturnT = ReturnType;
		using InternalFunctionT = ReturnType(*)(Args...);

		Function() noexcept = default;
		Function(const Function& other) noexcept = default;
		Function(Function&& other) noexcept = default;
		~Function() noexcept = default;

		Function& operator = (const Function& other) noexcept = default;
		Function& operator = (Function&& other) noexcept = default;

		ThisType& operator = (InternalFunctionT newInternalFunction) { internalFunction = newInternalFunction; return *this; }

		auto getInternalFunction() const noexcept { return internalFunction; }

		bool isValid() const noexcept { return internalFunction != nullptr; }

		operator bool() const noexcept { return isValid(); }

		void invalidate() noexcept { internalFunction = nullptr; }

		template<class ...Args>
		ReturnT invoke(Args&&... args) { return std::invoke(internalFunction, std::forward<Args>(args)...); }

	private:

		InternalFunctionT internalFunction = nullptr;

	};

}