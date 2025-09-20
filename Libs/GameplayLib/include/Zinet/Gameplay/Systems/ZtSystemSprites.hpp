#pragma once

#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClock.hpp"

#include "Zinet/Math/ZtMVP.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::gameplay
{
	// TODO: Rename to SystemInstancedSprites
	class  SystemSprites : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemSprites");

	public:

		SystemSprites() = default;
		SystemSprites(const SystemSprites& other) = default;
		SystemSprites(SystemSprites&& other) noexcept = default;
		~SystemSprites() noexcept = default;

		SystemSprites& operator = (const SystemSprites& other) = default;
		SystemSprites& operator = (SystemSprites&& other) noexcept = default;

		bool deinit() override;

		void update() override;

		void addNode(const ObjectWeakHandle<Node>& node) override;

		auto& getTransforms() const noexcept { return transforms; }
		auto& getTransforms() noexcept { return transforms; }

		vulkan_renderer::DescriptorInfo getDescriptorInfo();

	protected:

		bool isDirty = true;

		std::vector<uint32_t> ids;

		std::vector<vulkan_renderer::Transform> transforms;
		std::vector<vulkan_renderer::Transform::MatrixT> transformsMatrices;
		vulkan_renderer::Buffer transformsMatricesBuffer{ nullptr };
		void recreateTransformsBuffer();
		void updateTransformsBufferData();

	};

}