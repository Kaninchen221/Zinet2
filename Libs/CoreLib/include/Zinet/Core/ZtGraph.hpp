#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{
	class Graph : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("zt::core::Graph");

	public:

		using ValueT = float;

		Graph() = default;
		Graph(ValueT newMinValue, ValueT newMaxValue, size_t newMaxPoints) : minValue{ newMinValue }, maxValue{ newMaxValue }, maxPoints{ newMaxPoints }
		{
			Ensure(maxPoints > 0, "maxPoints must be greater than 0");
			Ensure(maxValue >= minValue, "maxValue must be greater than minValue");
		};
		Graph(const Graph& other) = delete;
		Graph(Graph&& other) noexcept = default;

		Graph& operator = (const Graph& other) = delete;
		Graph& operator = (Graph&& other) noexcept = default;

		~Graph() noexcept = default;

		const auto& getValues() const noexcept { return values; }

		void update(const ValueT newValue) noexcept;

		void show() override;

		void setMinValue(const ValueT newMinValue) noexcept;
		auto getMinValue() const noexcept { return minValue; }

		void setMaxValue(const ValueT newMaxValue) noexcept;
		auto getMaxValue() const noexcept { return maxValue; }

		void setMaxPoints(const size_t newMaxPoints) noexcept;
		auto getMaxPoints() const noexcept { return maxPoints; }

	protected:

		std::vector<ValueT> values;
		ValueT minValue{};
		ValueT maxValue{};
		size_t maxPoints{ 1000 };

	};

}