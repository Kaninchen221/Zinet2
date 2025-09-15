#include "Zinet/Core/ZtGraph.hpp"

#include "Zinet/Core/ZtImgui.hpp"

#include <implot.h>

#include <fmt/format.h>

namespace zt::core
{
	void Graph::ShowPlotAny()
	{
		ImGui::Checkbox("Plot Any", &PlotAny);
	}

	void Graph::update(const ValueT newValue) noexcept
	{
		if (!plot || !PlotAny)
			return;

		while (values.size() >= maxPoints)
		{
			values.erase(values.begin());
		}

		// Calc average value
		if (values.empty()) 
		{
			average = newValue;
		}
		else
		{
			const ValueT diff = newValue - average;
			average += diff / values.size();
		}

		values.push_back(newValue);

		if (largestValue < newValue)
			largestValue = newValue;
		else if (smallestValue > newValue)
			smallestValue = newValue;
	}

	void Graph::show()
	{
		ImGui::PushID(this);

		auto checkboxLabel = fmt::format("Plot {}", displayName);
		ImGui::Checkbox(checkboxLabel.c_str(), &plot);

		ImGui::TextFMT("Largest: {} Smallest: {} Average: {}", largestValue, smallestValue, average);

		if (ImGui::Button("Reset"))
			reset();

		auto plotLabel = fmt::format("##{}", checkboxLabel);
		static ImPlotAxisFlags plotFlags = ImPlotFlags_NoFrame | ImPlotFlags_NoLegend;
		static ImPlotAxisFlags axisFlags = ImPlotAxisFlags_NoTickLabels;// | ImPlotAxisFlags_AutoFit;
		if (ImPlot::BeginPlot(plotLabel.c_str(), ImVec2(-1, 140), plotFlags))
		{
			ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
			ImPlot::SetupAxisLimits(
				ImAxis_Y1, 
				resizeToSmallest ? smallestValue : minValue,
				resizeToLargest ? largestValue : maxValue, 
				ImPlotCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0, static_cast<double>(values.size()), ImPlotCond_Always);
			ImPlot::PlotLine(displayName.c_str(), values.data(), static_cast<int>(values.size()));
			ImPlot::EndPlot();
		}

		ImGui::PopID();
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

	void Graph::reset()
	{
		values.clear();
		smallestValue = std::numeric_limits<ValueT>::max();
		largestValue = std::numeric_limits<ValueT>::lowest();
		average = 0;
	}

}