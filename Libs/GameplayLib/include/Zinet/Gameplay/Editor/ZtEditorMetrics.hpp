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

		void gpuMetrics();
		void gpuMemoryMetrics();

	};
}