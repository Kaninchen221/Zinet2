#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{

	void ConsoleLogger::turnOff()
	{
		lastLevel = internal->level();
		internal->set_level(spdlog::level::off);
	}

	void ConsoleLogger::turnOn()
	{
		internal->set_level(lastLevel);
	}

}