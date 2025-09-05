#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class SystemWindow : public System
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemWindow");

	public:

		SystemWindow() = default;
		SystemWindow(const SystemWindow& other) = default;
		SystemWindow(SystemWindow&& other) = default;
		~SystemWindow() = default;

		SystemWindow& operator = (const SystemWindow& other) = default;
		SystemWindow& operator = (SystemWindow&& other) = default;

		bool init() override;

		void update() override;

		bool deinit() override;

		auto& getWindow() noexcept { return window; }
		auto& getWindow() const noexcept { return window; }

	protected:

		wd::Window window;
		wd::WindowEvents windowEvents{ window };
	};

}