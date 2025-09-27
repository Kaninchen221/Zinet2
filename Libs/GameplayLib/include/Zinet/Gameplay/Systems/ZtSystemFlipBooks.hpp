#pragma once

#include "Zinet/Gameplay/Systems/ZtSystem.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Nodes/ZtNodeInstancedFlipBook.hpp"

#include "Zinet/Core/ZtClock.hpp"

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

		bool init() override;

		bool deinit() override;

		void addNode(const ObjectWeakHandle<Node>& node) override;

		vulkan_renderer::DescriptorInfo getDescriptorInfo();

		auto& getFlipBooksFrames() noexcept { return flipBooksFrames; }
		auto& getFlipBooksFrames() const noexcept { return flipBooksFrames; }

	protected:

		using FlipBooksFrames = std::vector<FlipBookFrames>;
		FlipBooksFrames flipBooksFrames;
		std::vector<int32_t> currentFramesIndices;
		std::vector<FlipBookFrame::Coords> currentFramesCoords;
		std::vector<float> framesTimeouts; // TODO: Change the type to core::Time
		vulkan_renderer::Buffer frameTexCoordsBuffer{ nullptr };
		void recreateFrameTexCoordsBuffer();

		core::Clock clock;
	};

}