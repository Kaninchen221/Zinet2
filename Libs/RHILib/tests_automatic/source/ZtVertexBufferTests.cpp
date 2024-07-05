#pragma once

#ifdef ZINET_USE_OPENGL

#include "Zinet/RHI/OpenGL/ZtVertexBuffer.hpp"
#include "Zinet/RHI/OpenGL/ZtOpenGLResolver.hpp"

#include "Zinet/RHI/ZtVertex.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::opengl::tests
{

	class VertexBufferTests : public ::testing::Test
	{
	protected:

		VertexBufferTests()
		{
		}

		~VertexBufferTests() override
		{
		}

		void SetUp() override
		{
			window.create();
			OpenGLResolver::Init();
		}

		void TearDown() override
		{
		}

		wd::GLFW glfw;
		wd::Window window;
		VertexBuffer vertexBuffer;
	};

	TEST(VertexBufferSimpleTest, GetInvalidID)
	{
		EXPECT_EQ(0u, VertexBuffer::GetInvalidID());
	}

	TEST(VertexBufferSimpleTest, GetID)
	{
		VertexBuffer vertexBuffer;
		const ZtUInt actualID = vertexBuffer.getID();
		EXPECT_EQ(actualID, VertexBuffer::GetInvalidID());
	}

	TEST_F(VertexBufferTests, Create)
	{
		EXPECT_TRUE(vertexBuffer.create());
	}

	TEST_F(VertexBufferTests, Bind)
	{
		EXPECT_TRUE(vertexBuffer.create());
		EXPECT_TRUE(vertexBuffer.bind());

		const ZtUInt currentBoundVertexBufferID = VertexBuffer::GetCurrentBoundVertexBufferID();
		EXPECT_EQ(currentBoundVertexBufferID, vertexBuffer.getID());
	}

	TEST_F(VertexBufferTests, CopyVerticesFromRange)
	{
		EXPECT_TRUE(vertexBuffer.create());
		EXPECT_TRUE(vertexBuffer.bind());

		const std::vector<Vertex> vertices = { Vertex{}, Vertex{} };
		vertexBuffer.copyDataFromVector(vertices);
	}
}

#endif // ZINET_USE_OPENGL