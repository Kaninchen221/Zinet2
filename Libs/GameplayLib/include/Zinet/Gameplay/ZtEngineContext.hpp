#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API EngineContext
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EngineContext");

	public:

		EngineContext() ZINET_API_POST {
			instance = this;
		};
		EngineContext(const EngineContext& other) ZINET_API_POST = default;
		EngineContext(EngineContext&& other) ZINET_API_POST = default;
		~EngineContext() ZINET_API_POST;

		EngineContext& operator = (const EngineContext& other) ZINET_API_POST = default;
		EngineContext& operator = (EngineContext&& other) ZINET_API_POST = default;

		static auto& Get() ZINET_API_POST { core::Ensure(instance); return *instance; }

		bool init() ZINET_API_POST;

		void loop() ZINET_API_POST;

		void deinit() ZINET_API_POST;

		template<std::derived_from<System> SystemT>
		void addSystem() ZINET_API_POST
		{
			systems.emplace_back(std::make_shared<SystemT>());
		}

		template<std::derived_from<System> SystemT>
		SystemT* getSystem() ZINET_API_POST
		{
			for (auto& system : systems)
			{
				auto ptr = dynamic_cast<SystemT*>(system.get());
				if (ptr)
					return ptr;
			}

			return nullptr;
		}

		auto& getSystems() ZINET_API_POST { return systems; }
		const auto& getSystems() const ZINET_API_POST { return systems; }

		vulkan_renderer::ImGuiIntegration imGuiIntegration;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };

		core::assets::AssetsStorage assetsStorage;

		NodeHandle<Node> rootNode;

	private:

		void destroyNodes(NodeHandle<> node) ZINET_API_POST;

		inline static EngineContext* instance = nullptr;

		bool initialized = false;

		using Systems = std::vector<std::shared_ptr<System>>;
		Systems systems;

	};
}