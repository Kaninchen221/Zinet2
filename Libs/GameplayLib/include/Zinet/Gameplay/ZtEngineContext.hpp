#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClassRegistry.hpp"
#include "Zinet/Core/ZtObjectsStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class EngineContext
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EngineContext");

		inline static EngineContext* instance = nullptr;

	public:
		using Systems = std::vector<ObjectHandle<System>>;

		EngineContext() {
			instance = this;
		};
		EngineContext(const EngineContext& other) = default;
		EngineContext(EngineContext&& other) noexcept = default;
		~EngineContext() noexcept;

		EngineContext& operator = (const EngineContext& other) = default;
		EngineContext& operator = (EngineContext&& other) noexcept = default;

		// TODO: Change it to an object handle to be consistent with rest of the code
		static auto& Get() noexcept { Ensure(instance); return *instance; }

		bool init();

		void loop();

		void deinit();

		// TODO: Return object handle to the created system
		template<std::derived_from<System> SystemT>
		void addSystem(const std::string_view& displayName);

		// TODO: Replace raw pointer with object handle
		template<std::derived_from<System> SystemT>
		SystemT* getSystem()
		{
			for (auto& system : systems)
			{
				auto ptr = dynamic_cast<SystemT*>(system.get());
				if (ptr)
					return ptr;
			}

			return nullptr;
		}

		auto& getWindow() noexcept { return window; }
		auto& getWindow() const noexcept { return window; }

		auto& getWindowEvents() noexcept { return windowEvents; }
		auto& getWindowEvents() const noexcept { return windowEvents; }

		auto& getAssetsStorage() noexcept { return assetsStorage; }
		auto& getAssetsStorage() const noexcept { return assetsStorage; }

		auto& getObjectsStorage() noexcept { return objectsStorage; }
		auto& getObjectsStorage() const noexcept { return objectsStorage; }

		auto& getClassRegistry() noexcept { return classRegistry; }
		auto& getClassRegistry() const noexcept { return classRegistry; }

		auto& getRootNode() noexcept { return rootNode; }
		auto& getRootNode() const noexcept { return rootNode; }

		auto& getSystems() noexcept { return systems; }
		auto& getSystems() const noexcept { return systems; }

		bool isInitialized() const noexcept { return initialized; }

	protected:

		wd::Window window;
		wd::WindowEvents windowEvents{ window };
		core::AssetsStorage assetsStorage;
		core::ObjectsStorage objectsStorage;
		core::ClassRegistry<core::Object> classRegistry;
		ObjectHandle<Node> rootNode;
		Systems systems;

		void destroyNodes(ObjectHandle<Node>& node);

		bool initialized = false;


	};
}

/// Not in "core" namespace because used too often
namespace zt
{
	template<std::derived_from<core::Object> ObjectT>
	auto CreateObject(const std::string_view& displayName)
	{
		auto& engineContext = gameplay::EngineContext::Get();
		return engineContext.getObjectsStorage().createObject<ObjectT>(displayName);
	}
}

namespace zt::gameplay
{
	template<std::derived_from<System> SystemT>
	void EngineContext::addSystem(const std::string_view& displayName)
	{
		auto system = CreateObject<SystemT>(displayName);
		systems.emplace_back(system);
	}
}