#include "Zinet/Core/ZtGraph.hpp"

#include <imgui.h>
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

		values.push_back(newValue);
	}

	void Graph::show()
	{
		auto checkboxLabel = fmt::format("Plot {}", displayName);
		ImGui::Checkbox(checkboxLabel.c_str(), &plot);

		auto plotLabel = fmt::format("##{}", checkboxLabel);
		static ImPlotAxisFlags plotFlags = ImPlotFlags_NoFrame | ImPlotFlags_NoLegend;
		static ImPlotAxisFlags axisFlags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit;
		if (ImPlot::BeginPlot(plotLabel.c_str(), ImVec2(-1, 140), plotFlags))
		{
			ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
			ImPlot::PlotLine(displayName.c_str(), values.data(), static_cast<int>(values.size()));
			ImPlot::EndPlot();
		}
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