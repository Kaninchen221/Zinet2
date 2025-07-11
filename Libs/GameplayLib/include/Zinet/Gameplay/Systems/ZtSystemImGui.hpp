#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API SystemImGui : public System<Node>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemImGui");

	public:

		using ClassBaseT = System<Node>;

		SystemImGui() noexcept = default;
		SystemImGui(const SystemImGui& other) noexcept = default;
		SystemImGui(SystemImGui&& other) noexcept = default;
		~SystemImGui() noexcept = default;

		SystemImGui& operator = (const SystemImGui& other) noexcept = default;
		SystemImGui& operator = (SystemImGui&& other) noexcept = default;

		bool init() noexcept override;

		void deinit() noexcept override;

		void addNode(const NodeWeakHandle<NodeT>& node) noexcept;

		void update() noexcept;

	};

}