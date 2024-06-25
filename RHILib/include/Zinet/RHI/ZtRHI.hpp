#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"

namespace zt::RHI
{

	class ZINET_RHI_API RHI
	{

	public:

		RHI() = default;
		RHI(const RHI& other) = default;
		RHI(RHI&& other) = default;

		RHI& operator = (const RHI& other) = default;
		RHI& operator = (RHI&& other) = default;

		~RHI() noexcept = default;

		void placeholder();

	};

}