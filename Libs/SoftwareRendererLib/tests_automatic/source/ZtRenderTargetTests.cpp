#pragma once

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{

	class RenderTargetTests : public ::testing::Test
	{
	protected:

		RenderTargetTests()
		{
		}

		~RenderTargetTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		RenderTarget renderTarget;
	};

	TEST_F(RenderTargetTests, Test)
	{
		// Create render target (It contains random data from memory)
		const Vector2i expectedResolution = { 8, 12 };
		const ColorFormat expectedColorFormat = ColorFormat::R8G8B8A8_SRGB;
		const bool renderTargetCreateResult = renderTarget.createEmpty(expectedResolution, expectedColorFormat);

		// Test properties
		ASSERT_TRUE(renderTargetCreateResult);
		EXPECT_EQ(renderTarget.getResolution(), expectedResolution);
		EXPECT_EQ(renderTarget.getColorFormat(), expectedColorFormat);

		// Fill 
		const Color fillColor = { 100u, 254u, 30u, 255u };

		const bool fillResult = renderTarget.fill(fillColor);
		ASSERT_TRUE(fillResult);

		// Test the last pixel
		const size_t lastPixelIndex = expectedResolution.x * expectedResolution.y - 1u;
		const Color pixelColor = renderTarget.getPixelColor(lastPixelIndex);
		ASSERT_EQ(pixelColor, fillColor);

		// Save it before clear
		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "render_target_test_result.png";
		const bool saveToPNGResult = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveToPNGResult);

		// Clear, so we can test load result
		renderTarget.clear();
		EXPECT_EQ(renderTarget.getResolution(), Vector2i( 0u, 0u ));
		EXPECT_EQ(renderTarget.getChannels(), 0u);

		// Load from file
		const bool loadFromPNGResult = renderTarget.loadFromFilePNG(path);
		ASSERT_TRUE(loadFromPNGResult);

		// Test last pixel after load
		const Color pixelColorAfterLoad = renderTarget.getPixelColor(lastPixelIndex);
		ASSERT_EQ(pixelColorAfterLoad, fillColor);

		// Change color of one pixel
		const size_t writePixelIndex = 1u;
		const Color writeColor = { 10u, 12u, 255u, 200u };
		renderTarget.writePixelColor(writePixelIndex, writeColor);

		// Test the changed pixel color
		const Color changedPixelColor = renderTarget.getPixelColor(writePixelIndex);
		ASSERT_EQ(changedPixelColor, writeColor);

		// Save it for manual test
		const bool saveToPNGResultAfterWritePixel = renderTarget.saveToFilePNG(path);
		ASSERT_TRUE(saveToPNGResultAfterWritePixel);
	}

	TEST_F(RenderTargetTests, CopyTest)
	{
		const Vector2i expectedResolution = { 4, 3 };
		const ColorFormat expectedColorFormat = ColorFormat::R8G8B8A8_SRGB;
		const bool renderTargetCreateResult = renderTarget.createEmpty(expectedResolution, expectedColorFormat);
		renderTarget.fill(WhiteColor);
		ASSERT_TRUE(renderTargetCreateResult);
		
		const Pixel expectedPixel = Pixel{ .coords{ 1, 0 }, .color = BlackColor };
		renderTarget.writePixel(expectedPixel);

		const RenderTarget renderTargetCopy = renderTarget;
		const Color actualColor = renderTargetCopy.getPixelColor(expectedPixel.coords);
		ASSERT_EQ(expectedPixel.color, actualColor);

		const Color firstPixelColor = renderTargetCopy.getPixelColor({ 0, 0 });
		ASSERT_EQ(firstPixelColor, WhiteColor);

		const std::filesystem::path path = core::Paths::CurrentProjectRootPath() / "test_files" / "copy_render_target_test.png";
		const bool saveToPNGResult = renderTargetCopy.saveToFilePNG(path);
		ASSERT_TRUE(saveToPNGResult);
	}
}