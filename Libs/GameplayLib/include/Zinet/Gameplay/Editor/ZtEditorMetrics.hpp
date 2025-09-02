#pragma once

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtGraph.hpp"

namespace zt::gameplay
{
	// TODO: Extend it. We need better tools to measure performance inside the engine
	struct EditorMetrics
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorMetrics");

	public:

		bool shouldShow = false;
		void show();

	protected:

		void imGuiMetrics();
		core::Graph imGuiFPSGraph{ "ImGui FPS", 0.f, 0.f, 1000};

		void cpuMetrics();

		void gpuMetrics();
		void gpuMemoryMetrics();

	};
}