#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

	class RendererTests : public ::testing::Test
	{
	protected:

		RendererTests()
		{
		}

		~RendererTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Renderer renderer;
		const Renderer& constRenderer = renderer;

		RendererContext& rendererContext = renderer.getRendererContext();
		const RendererContext& constRendererContext = renderer.getRendererContext();
	};

	TEST_F(RendererTests, EmptyTest)
	{}
}

#endif // ZINET_USE_OPENGL