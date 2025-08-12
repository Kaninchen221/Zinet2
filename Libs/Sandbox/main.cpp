#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Sandbox/ZtSandbox.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	auto callback = []() { zt::TerminateDebug(); };
    zt::core::CustomSink::SetCallback(callback);

	zt::sandbox::Sandbox sandbox;
	sandbox.start();

	return 0;
}