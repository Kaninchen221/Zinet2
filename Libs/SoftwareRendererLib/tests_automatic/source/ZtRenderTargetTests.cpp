#pragma once

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	class RenderTargetTests : public ::testing::Test
	{
	protected:

		static_assert(sizeof(Texel) == sizeof(uint32_t), "Texel should be 4 bytes in size");

		inline static auto BigDimension = Vector2ui(10, 1'000'000);
		inline static auto Color = Texel{ 255, 0, 0, 255 };
	};

	TEST_F(RenderTargetTests, Create)
	{
		constexpr Vector2ui dimension(4, 4);
		const auto renderTarget = RenderTarget::Create(dimension);

		const auto& actualDimension = renderTarget.getDimension();
		EXPECT_EQ(dimension, actualDimension);

		constexpr size_t expectedSize = sizeof(Texel) * dimension.x * dimension.y;
		const size_t actualSize = renderTarget.getSize();
		EXPECT_EQ(expectedSize, actualSize);
	}

	TEST_F(RenderTargetTests, CreateBig)
	{
		const auto renderTarget = RenderTarget::Create(BigDimension);
	}

	TEST_F(RenderTargetTests, CreateWithColor)
	{
		const auto renderTarget = RenderTarget::Create(BigDimension, Color);

		ASSERT_TRUE(renderTarget.allTexelsAre(Color));
	}

	TEST_F(RenderTargetTests, Clear)
	{
		auto renderTarget = RenderTarget::Create(BigDimension);
		const Texel clearColor{ 133, 0, 255, 111 };
		renderTarget.clear(clearColor);

		ASSERT_TRUE(renderTarget.allTexelsAre(clearColor));
	}
}
