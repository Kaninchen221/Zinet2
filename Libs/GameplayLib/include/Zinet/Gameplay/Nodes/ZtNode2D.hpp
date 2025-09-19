#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/Math/ZtMVP.hpp"

namespace zt::gameplay
{
	class  Node2D : public Node
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Node2D");

	public:

		// TODO: Remove that 
		// The default texture should be passed from global descriptor set to shaders
		inline static AssetProperty<AssetTexture, core::Static::Yes> DefaultTexture{ "Default Texture" };

		Node2D() = default;
		Node2D(const Node2D& other) = default;
		Node2D(Node2D&& other) noexcept = default;
		~Node2D() noexcept = default;

		Node2D& operator = (const Node2D& other) = default;
		Node2D& operator = (Node2D&& other) noexcept = default;

		void onCreate() override;

		void onDestroy() override;

		void show() override;

		void update(float deltaTime) override;

		virtual vulkan_renderer::DescriptorInfo getDescriptorInfo();

		vulkan_renderer::Transform transform{};

	protected:

		void updateMVP();

		MVP mvp{};
		vulkan_renderer::Buffer MVPBuffer{ nullptr };

	};

}