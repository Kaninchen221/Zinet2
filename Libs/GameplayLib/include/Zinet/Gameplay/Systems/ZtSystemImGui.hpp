#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class  SystemImGui : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemImGui");

	public:

		SystemImGui() = default;
		SystemImGui(const SystemImGui& other) = default;
		SystemImGui(SystemImGui&& other) noexcept = default;
		~SystemImGui() noexcept = default;

		SystemImGui& operator = (const SystemImGui& other) = default;
		SystemImGui& operator = (SystemImGui&& other) noexcept = default;

		bool init() override;

		void deinit() override;

		void update() override;

	};

}