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

		SystemImGui() ZINET_API_POST = default;
		SystemImGui(const SystemImGui& other) ZINET_API_POST = default;
		SystemImGui(SystemImGui&& other) ZINET_API_POST = default;
		~SystemImGui() ZINET_API_POST = default;

		SystemImGui& operator = (const SystemImGui& other) ZINET_API_POST = default;
		SystemImGui& operator = (SystemImGui&& other) ZINET_API_POST = default;

		bool init() ZINET_API_POST override;

		void deinit() ZINET_API_POST override;

		void addNode(const NodeWeakHandle<NodeT>& node) ZINET_API_POST;

		void update() ZINET_API_POST;

	};

}