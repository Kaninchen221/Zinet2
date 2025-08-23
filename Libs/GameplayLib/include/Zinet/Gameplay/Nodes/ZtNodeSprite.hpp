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
	class NodeSprite : public Node2D
	{
	private:


	public:

		NodeSprite() = default;
		NodeSprite(const NodeSprite& other) = default;
		NodeSprite(NodeSprite&& other) noexcept = default;
		~NodeSprite() noexcept = default;

		NodeSprite& operator = (const NodeSprite& other) = default;
		NodeSprite& operator = (NodeSprite&& other) noexcept = default;

		void imGui() override;

		vulkan_renderer::DescriptorInfo getPipelineDescriptorInfos() override;

		AssetProperty<AssetTexture> texture{ "Texture" };

	protected:



	};
}