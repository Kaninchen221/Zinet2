#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{
	void NodeCamera::onCreate(ObjectWeakHandle<Object> newSelf)
	{
		using namespace vulkan_renderer;

		Node::onCreate(newSelf);

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("System renderer is invalid");
			return;
		}

		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& vma = rendererContext.getVMA();
		BufferData data{ .view = camera.getViewMatrix(), .perspective = camera.getPerspectiveMatrix() };
		const auto createInfo = Buffer::GetUniformBufferCreateInfo(data);
		if (!buffer.create(vma, createInfo))
		{
			Logger->error("Couldn't create buffer");
			return;
		}

		if (!buffer.fillWithObject(vma, data))
		{
			Logger->error("Couldn't fill buffer");
			return;
		}
	}

	void NodeCamera::onDestroy()
	{
		Node::onDestroy();

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("System renderer is invalid");
			return;
		}

		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& vma = rendererContext.getVMA();
		buffer.destroy(vma);
	}

	void NodeCamera::show()
	{
#		if ZINET_USE_IMGUI
		Node::show();

		if (ImGui::CollapsingHeader("Properties"))
		{
			ImGui::Indent();
			if (ImGui::Button("Reset camera"))
			{
				camera.setPosition(Vector3f{0, 0, 150});
				camera.setLookingAt(Vector3f{});
			}

			ImGui::SliderFloat("Sliders Limit", &slidersLimit, 0.f, 1000000.f);

			Vector3f position = camera.getPosition();
			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&position), -slidersLimit, slidersLimit);

			Vector3f lookingAt = camera.getLookingAt();
			ImGui::SliderFloat3("Looking At", reinterpret_cast<float*>(&lookingAt), -slidersLimit, slidersLimit);

			if (ImGui::Button(">"))
			{
				position.x += 10;
				lookingAt.x += 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("<"))
			{
				position.x -= 10;
				lookingAt.x -= 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("^"))
			{
				position.y += 10;
				lookingAt.y += 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("v"))
			{
				position.y -= 10;
				lookingAt.y -= 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("+"))
			{
				position.z -= 10;
				lookingAt.z -= 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("-"))
			{
				position.z += 10;
				lookingAt.z += 10;
			}

			camera.setPosition(position);
			camera.setLookingAt(lookingAt);

			ImGui::Unindent();
		}

#		endif 
	}

	void NodeCamera::update(float deltaTime)
	{
		Node::update(deltaTime);

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("System renderer is invalid");
			return;
		}

		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& vma = rendererContext.getVMA();

		BufferData data{ .view = camera.getViewMatrix(), .perspective = camera.getPerspectiveMatrix() };
		buffer.fillWithObject(vma, data);
	}

	vulkan_renderer::DescriptorInfo NodeCamera::getDescriptorInfo()
	{
		using namespace vulkan_renderer;

		DescriptorInfo descriptorInfo;
		descriptorInfo.buffersPacks.emplace_back().buffersPerType[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] = std::vector<Buffer*>{ &buffer };

		return descriptorInfo;
	}
}