#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"
#include "Zinet/GameplayLib/ZtCamera.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Window/ZtEvent.hpp"


namespace zt::gameplay_lib
{
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API System : public core::Object
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("System");

	public:

		virtual void tick(float deltaTime) {}

		void addNode(const std::weak_ptr<Node>& node) { nodes.push_back(node); }

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<std::weak_ptr<Node>> nodes;

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<System>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<System>(); *result = *this; return result; }
		
		System() = default;
		System(const System& other) = default;
		System(System&& other) = default;
		~System() noexcept = default;
		
		System& operator = (const System& other) = default;
		System& operator = (System&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "System"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(nodes)& getNodes() const { return nodes; }
		
/*GENERATED_CODE_END*/
	};

	// TODO: Move this child classes to seperate files so we could use code generator 
	class ZINET_GAMEPLAY_LIB_API TickableSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("TickableSystem");

	public:

		void tick(float deltaTime) override;

	protected:


	public:

/*GENERATED_CODE_START*/
/*GENERATED_CODE_END*/
	};

	class ZINET_GAMEPLAY_LIB_API DrawableSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("DrawableSystem");

	public:

		void tick(float deltaTime) override;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<Camera> currentCamera;

		sf::SoftwareRenderer softwareRenderer;

	public:

/*GENERATED_CODE_START*/

		const decltype(currentCamera)& getCurrentCamera() const { return currentCamera; }
		decltype(currentCamera)& getCurrentCamera() { return currentCamera; }
		void setCurrentCamera(const decltype(currentCamera)& newValue) { currentCamera = newValue; }

/*GENERATED_CODE_END*/
	};

	class ZINET_GAMEPLAY_LIB_API DragableSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("DragableSystem");

	public:

		void tick(float deltaTime) override;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<Camera> currentCamera;

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<wd::Event> event;

		std::weak_ptr<Node> dragedNode;
		Vector2f offset;

	public:

		/*GENERATED_CODE_START*/

		const decltype(currentCamera)& getCurrentCamera() const { return currentCamera; }
		decltype(currentCamera)& getCurrentCamera() { return currentCamera; }
		void setCurrentCamera(const decltype(currentCamera)& newValue) { currentCamera = newValue; }

		const decltype(event)& getEvent() const { return event; }
		decltype(event)& getEvent() { return event; }
		void setEvent(const decltype(event)& newValue) { event = newValue; }

		/*GENERATED_CODE_END*/
	};
}