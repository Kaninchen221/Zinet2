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
#include "Zinet/Gameplay/ZtEngineThread.hpp"
#include "Zinet/Gameplay/ZtThreadID.hpp"
#include "Zinet/Gameplay/ZtUpdatePhase.hpp"

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

		EngineContext();
		EngineContext(const EngineContext& other) = default;
		EngineContext(EngineContext&& other) noexcept = default;
		~EngineContext() noexcept;

		EngineContext& operator = (const EngineContext& other) = default;
		EngineContext& operator = (EngineContext&& other) noexcept = default;

		// TODO: Change it to an object handle to be consistent with rest of the code?
		static auto& Get() noexcept { Ensure(instance); return *instance; }

		bool init();
		
		void loop();

		bool isLooping() const;

		void stopLooping();

		void deinit();

		template<std::derived_from<System> SystemT>
		ObjectHandle<SystemT> addSystem(const std::string_view& displayName, UpdatePhase updatePhase = UpdatePhase::Main, ThreadID threadID = ThreadID::Main);

		template<std::derived_from<System> SystemT>
		ObjectHandle<SystemT> getSystem(ThreadID threadID = ThreadID::Main);

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

		EngineThread& getThreadByID(ThreadID threadID);

		EngineThread mainThread{ "Main Thread", ThreadID::Main };
		EngineThread renderingThread{ "Rendering Thread", ThreadID::RenderingThread };

		void destroyNodes(ObjectHandle<Node>& node);
		
		core::AssetsStorage assetsStorage;
		core::ObjectsStorage objectsStorage;
		core::ClassRegistry<core::Object> classRegistry;
		ObjectHandle<Node> rootNode;

		Systems systems;

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
	ObjectHandle<SystemT> EngineContext::addSystem(const std::string_view& displayName, UpdatePhase updatePhase, ThreadID threadID)
	{
		auto& thread = getThreadByID(threadID);
		auto system = thread.addSystem<SystemT>(displayName, updatePhase);

		return systems.emplace_back(system);
	}

	template<std::derived_from<System> SystemT>
	ObjectHandle<SystemT> EngineContext::getSystem(ThreadID threadID)
	{
		auto& thread = getThreadByID(threadID);
		auto system = thread.getSystem<SystemT>();

		// TODO: Treat the engine threads more like array of threads
		if (!system && threadID == ThreadID::Main)
		{
			Logger->debug("Couldn't find a system on threadID: {}. Try get it from the rendering thread", static_cast<size_t>(threadID));
			return renderingThread.getSystem<SystemT>();
		}

		return system;
	}
}