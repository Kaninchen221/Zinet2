#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::gameplay
{
	void Node2D::onCreate()
	{
		using namespace vulkan_renderer;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!Ensure(systemRenderer))
		{
			Logger->error("System renderer is invalid");
			return;
		}

		auto& vma = systemRenderer->getRenderer().getRendererContext().getVMA();

		const auto bufferCreateInfo = Buffer::GetUniformBufferCreateInfo<MVP>(mvp);
		if (!Ensure(MVPBuffer.createBuffer(vma, bufferCreateInfo)))
		{
			Logger->error("Failed to create MVP uniform buffer");
		}
		
	}

	void Node2D::onDestroy()
	{
		if (!MVPBuffer.isValid())
			return;

		auto& engineContext = EngineContext::Get();
		auto& vma = engineContext.getSystem<SystemRenderer>()->getRenderer().getRendererContext().getVMA();
		MVPBuffer.destroy(vma);
	}

	void Node2D::show()
	{
		Node::show();
		transform.show();
	}

	void Node2D::update([[maybe_unused]] float deltaTime)
	{
		updateMVP();
	}

	vulkan_renderer::DescriptorInfo Node2D::getPipelineDescriptorInfos()
	{
		return {};
	}

	vulkan_renderer::DescriptorInfo Node2D::getDrawCallDescriptorInfos()
	{
		if (!MVPBuffer.isValid())
			return {};

		return vulkan_renderer::DescriptorInfo
		{
			.buffersPerType =
			{
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, { &MVPBuffer } }
			}
		};
	}

	void Node2D::updateMVP()
	{
		if (!MVPBuffer.isValid())
			return;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		const auto& camera = systemRenderer->getCameraNode();
		if (!camera)
		{
			Logger->warn("No camera set in SystemRenderer");
			return;
		}

		mvp.model = transform.getMatrix();
		mvp.view = camera->getCamera().getViewMatrix();
		mvp.projection = camera->getCamera().getPerspectiveMatrix();

		auto& vma = engineContext.getSystem<SystemRenderer>()->getRenderer().getRendererContext().getVMA();
		MVPBuffer.fillWithObject(vma, mvp);
	}

}