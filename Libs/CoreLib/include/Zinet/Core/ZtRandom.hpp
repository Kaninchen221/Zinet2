#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <random>
#include <concepts>

namespace zt::core
{
	class Random
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::Random");

	public:

		Random() : generator{ rd() } {}
		Random(const Random& other) = default;
		Random(Random&& other) noexcept = default;

		Random& operator = (const Random& other) = default;
		Random& operator = (Random&& other) noexcept = default;

		~Random() noexcept = default;

		template<std::floating_point NumberT>
		NumberT real(NumberT min, NumberT max)
		{
			std::uniform_real_distribution<NumberT> dist(min, max);
			return dist(generator);
		}

		template<std::integral NumberT>
		NumberT integer(NumberT min, NumberT max)
		{
			std::uniform_int_distribution<NumberT> dist(min, max);
			return dist(generator);
		}

	protected:

		std::random_device rd;
		std::mt19937 generator;

	};

}