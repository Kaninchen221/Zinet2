#include "Zinet/Core/ZtGraph.hpp"

#include <imgui.h>

#include <fmt/format.h>

namespace zt::core
{
	void Graph::update(const ValueT newValue) noexcept
	{
		while (values.size() >= maxPoints)
		{
			values.erase(values.begin());
		}

		values.push_back(newValue);
	}

	void Graph::show()
	{
		const int valuesOffset = 0;
		const ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, 80.0f);
		ImGui::PlotLines(
			"##0", 
			values.data(), 
			static_cast<int>(values.size()), 
			valuesOffset, 
			fmt::format("Points: {}", values.size()).c_str(), 
			minValue, maxValue, 
			size
		);
	}

	void Graph::setMinValue(const ValueT newMinValue) noexcept
	{
		if (Ensure(newMinValue <= maxValue, "newMinValue must be less than or equal to maxValue"))
			minValue = newMinValue;
	}

	void Graph::setMaxValue(const ValueT newMaxValue) noexcept
	{
		if (Ensure(newMaxValue >= minValue, "newMaxValue must be greater than or equal to minValue"))
			maxValue = newMaxValue;
	}

	void Graph::setMaxPoints(const size_t newMaxPoints) noexcept
	{
		if (Ensure(newMaxPoints > 0, "newMaxPoints must be greater than 0"))
			maxPoints = newMaxPoints;
	}

}