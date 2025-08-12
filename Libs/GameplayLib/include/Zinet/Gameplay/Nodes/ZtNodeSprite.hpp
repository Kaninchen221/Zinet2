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
	class  NodeSprite : public Node2D
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeSprite");

	public:

		NodeSprite() = default;
		NodeSprite(const NodeSprite& other) = default;
		NodeSprite(NodeSprite&& other) noexcept = default;
		~NodeSprite() noexcept = default;

		NodeSprite& operator = (const NodeSprite& other) = default;
		NodeSprite& operator = (NodeSprite&& other) noexcept = default;

		void imGui() override;

		AssetProperty<AssetTexture> texture{ "Texture" };

	};
}