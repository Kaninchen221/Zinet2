#pragma once

#include "Zinet/GameplayLib/ZtFlipbook.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class FlipbookTests : public ::testing::Test
	{
	protected:

		FlipbookTests()
		{
		}

		~FlipbookTests() override {
		}

		void SetUp() override 
		{
			FlipbookFrame frame1
			{
				.texture = texture,
				.textureRegion = { { 0.f, 0.f }, { 1.f, 1.f } },
				.time = timePerFrame
			};

			FlipbookFrame frame2 = frame1;

			flipbook.addFrame(frame1);
			flipbook.addFrame(frame2);
		}

		void TearDown() override {
		}

		void testCurrentFrame(const size_t expectedFrameIndex) const
		{
			const size_t currentFrameIndex = flipbook.getCurrentFrameIndex();
			ASSERT_EQ(currentFrameIndex, expectedFrameIndex);
		}

		const float timePerFrame = 1000.f; // ms

		Flipbook flipbook;
		sf::RenderTarget texture;
	};

	TEST_F(FlipbookTests, FramesTest)
	{
		const auto& frames = flipbook.getFrames();
		ASSERT_EQ(frames.size(), 2);
	}

	TEST_F(FlipbookTests, UpdateTest)
	{
		testCurrentFrame(0u);

		flipbook.update(500.f);

		testCurrentFrame(0u);

		flipbook.update(1000.f);

		testCurrentFrame(1u);

		flipbook.update(1000.f);

		testCurrentFrame(0u);
	}

	TEST_F(FlipbookTests, GetDrawInfoTest)
	{
		sf::DrawInfo drawInfo = flipbook.getDrawInfo();
	}
}