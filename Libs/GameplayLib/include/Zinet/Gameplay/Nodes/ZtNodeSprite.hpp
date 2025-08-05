#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetProperty.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API NodeSprite : public Node2D
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeSprite");

	public:

		NodeSprite() ZINET_API_POST = default;
		NodeSprite(const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite(NodeSprite&& other) ZINET_API_POST = default;
		~NodeSprite() ZINET_API_POST = default;

		NodeSprite& operator = (const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite& operator = (NodeSprite&& other) ZINET_API_POST = default;

		void imGui() ZINET_API_POST override;

		AssetProperty<AssetTexture> texture{ "Texture" };

	};
}