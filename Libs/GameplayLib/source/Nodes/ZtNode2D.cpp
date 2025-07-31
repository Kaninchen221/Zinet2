#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"

namespace zt::gameplay
{
	vulkan_renderer::DescriptorInfo Node2D::getPipelineDescriptorInfos() const ZINET_API_POST
	{
		return {};
	}

	vulkan_renderer::DescriptorInfo Node2D::getDrawCallDescriptorInfos() const ZINET_API_POST
	{
		return {};
	}


}