#pragma once

#include "Zinet/RHI/ZtVertex.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::tests
{

	class VertexTests : public ::testing::Test
	{
	protected:

		VertexTests()
		{
		}

		~VertexTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Vertex vertex;
	};

	TEST_F(VertexTests, GetSetPosition)
	{
		const Vector4f ExpectedPosition = { 123.f, 1.34f, 23.f, 1.f };
		vertex.setPosition(ExpectedPosition);
		const Vector4f& ActualPosition = vertex.getPosition();

		EXPECT_EQ(ExpectedPosition, ActualPosition);
	}

	TEST_F(VertexTests, GetSetColor)
	{
		const Vector4f ExpectedColor = { 123.f, 1.34f, 23.f, 1.f };
		vertex.setColor(ExpectedColor);
		const Vector4f& ActualColor = vertex.getColor();

		EXPECT_EQ(ExpectedColor, ActualColor);
	}

	TEST_F(VertexTests, GetUV)
	{
		const Vector2f ExpectedUV = { 1.f, 0.5f };
		vertex.setUV(ExpectedUV);
		const Vector2f& ActualUV = vertex.getUV();

		EXPECT_EQ(ExpectedUV, ActualUV);
	}
	
}