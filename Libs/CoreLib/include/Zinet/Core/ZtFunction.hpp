#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <type_traits>

namespace zt::core
{
	template<class ReturnType, class... Args>
	class Function
	{
	public:

		using ThisType = Function<ReturnType, Args...>;
		using ReturnT = ReturnType;
		using InternalFunctionT = ReturnType(*)(Args...);

		Function() noexcept = default;
		Function(auto callable) : internalFunction{ callable } {}
		Function(InternalFunctionT newInternalFunction) : internalFunction{ newInternalFunction } {}
		Function(const Function& other) noexcept = default;
		Function(Function&& other) noexcept = default;
		~Function() noexcept = default;

		Function& operator = (const Function& other) noexcept = default;
		Function& operator = (Function&& other) noexcept = default;

		auto getInternalFunction() const noexcept { return internalFunction; }

		bool isValid() const noexcept { return internalFunction != nullptr; }

		operator bool() const noexcept { return isValid(); }

		void invalidate() noexcept { internalFunction = nullptr; }

		template<class ...Args>
		ReturnT invoke(Args&&... args) const { return std::invoke(internalFunction, std::forward<Args>(args)...); }

	private:

		InternalFunctionT internalFunction = nullptr;

	};

}