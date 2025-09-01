#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtGraph.hpp"

namespace zt::core::tests
{

	class GraphTests : public ::testing::Test
	{
	protected:

		const Graph::ValueT minValue = 0.f;
		const Graph::ValueT maxValue = 10.f;
		const size_t maxPoints = 2;
		Graph graph{ minValue, maxValue, maxPoints };
	};

	TEST_F(GraphTests, Test)
	{
		const std::vector<Graph::ValueT>& values = graph.getValues();
		ASSERT_TRUE(values.empty());

		graph.update(0);
		graph.update(1);
		graph.update(2);
		graph.update(3);
		graph.update(4);
		graph.update(5);
		graph.update(6);

		ASSERT_EQ(values, std::vector<Graph::ValueT>({ 5, 6 }));
	}
}