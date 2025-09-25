#pragma once

#include "Zinet/Gameplay/Systems/ZtSystem.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

namespace zt::gameplay
{
	class  SystemFlipBooks : public SystemSprites
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemFlipBooks");

	public:

		SystemFlipBooks() = default;
		SystemFlipBooks(const SystemFlipBooks& other) = default;
		SystemFlipBooks(SystemFlipBooks&& other) noexcept = default;
		~SystemFlipBooks() noexcept = default;

		SystemFlipBooks& operator = (const SystemFlipBooks& other) = default;
		SystemFlipBooks& operator = (SystemFlipBooks&& other) noexcept = default;

		void update() override;

		bool deinit() override;

		void addNode(const ObjectWeakHandle<Node>& node) override;

		vulkan_renderer::DescriptorInfo getDescriptorInfo();

	protected:



	};

}