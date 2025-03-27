#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtCamera.hpp"

#include "Zinet/GameplayLib/Systems/ZtSystem.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Window/ZtEvent.hpp"

namespace zt::gameplay_lib
{
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API WindowEventsSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("WindowEventsSystem");

	public:

		void tick(float deltaTime) override;

		void addDragableNode(const std::weak_ptr<Node>& node) { dragableNodes.push_back(node); }

		using ClickedNodeCallBackT = std::function<void(const std::weak_ptr<Node>)>;

		void addClickableNode(const std::weak_ptr<Node>& node, const ClickedNodeCallBackT& callback) { clickableNodes.push_back(node); clickableNodesCallbacks.push_back(callback); }

	protected:

		void tickDragable(float deltaTime);
		void tickClickable(float deltaTime);

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<Camera> currentCamera;

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<wd::Event> event;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<std::weak_ptr<Node>> dragableNodes;

		std::weak_ptr<Node> dragedNode;
		Vector2f offset;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<std::weak_ptr<Node>> clickableNodes;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<ClickedNodeCallBackT> clickableNodesCallbacks;

	public:

		/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<WindowEventsSystem>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<WindowEventsSystem>(); *result = *this; return result; }
		
		WindowEventsSystem() = default;
		WindowEventsSystem(const WindowEventsSystem& other) = default;
		WindowEventsSystem(WindowEventsSystem&& other) = default;
		~WindowEventsSystem() noexcept = default;
		
		WindowEventsSystem& operator = (const WindowEventsSystem& other) = default;
		WindowEventsSystem& operator = (WindowEventsSystem&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "WindowEventsSystem"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(currentCamera)& getCurrentCamera() const { return currentCamera; }
		decltype(currentCamera)& getCurrentCamera() { return currentCamera; }
		void setCurrentCamera(const decltype(currentCamera)& newValue) { currentCamera = newValue; }
		
		const decltype(event)& getEvent() const { return event; }
		decltype(event)& getEvent() { return event; }
		void setEvent(const decltype(event)& newValue) { event = newValue; }
		
		const decltype(dragableNodes)& getDragableNodes() const { return dragableNodes; }
		
		const decltype(clickableNodes)& getClickableNodes() const { return clickableNodes; }
		
		const decltype(clickableNodesCallbacks)& getClickableNodesCallbacks() const { return clickableNodesCallbacks; }
		
/*GENERATED_CODE_END*/
	};
}