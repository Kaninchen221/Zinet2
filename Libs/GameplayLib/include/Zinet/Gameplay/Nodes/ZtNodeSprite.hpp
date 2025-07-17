#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API NodeSprite : public Node2D
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeSprite");

	public:

		NodeSprite() ZINET_API_POST = delete;
		NodeSprite(const NodeNameView& newName) ZINET_API_POST : Node2D{ newName } {}
		NodeSprite(const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite(NodeSprite&& other) ZINET_API_POST = default;
		~NodeSprite() ZINET_API_POST = default;

		NodeSprite& operator = (const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite& operator = (NodeSprite&& other) ZINET_API_POST = default;

		void imGuiNodeInspect() ZINET_API_POST override;

		core::assets::AssetHandle<> texture;

	};
}