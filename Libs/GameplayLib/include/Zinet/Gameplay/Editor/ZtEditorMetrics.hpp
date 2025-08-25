#pragma once

#include "Zinet/Core/ZtLogger.hpp"

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
		float imGuiFPSMax = 0.f;
		float imGuiFPSMin = 0.f;
		size_t imGuiFPSMaxPoints = 10000;
		std::vector<float> imGuiFPSPoints = std::vector<float>(imGuiFPSMaxPoints);

		void cpuMetrics();

		void gpuMetrics();
		void gpuMemoryMetrics();

	};
}