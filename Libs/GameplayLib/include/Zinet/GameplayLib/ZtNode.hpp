#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API Node : public core::Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Node");

	public:

		virtual void tick(float deltaTime /*ms*/) {}

		virtual bool isDrawInfoDirty() const { return true; }

		virtual sf::DrawInfo getDrawInfo() const { return {}; }

		Vector2f getAbsolutePosition() const;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::weak_ptr<Node> parentNode;

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2f position{ 0, 0 };

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2f size{ 32, 32 };

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Node>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Node>(); *result = *this; return result; }
		
		Node() = default;
		Node(const Node& other) = default;
		Node(Node&& other) = default;
		~Node() noexcept = default;
		
		Node& operator = (const Node& other) = default;
		Node& operator = (Node&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Node"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		const decltype(parentNode)& getParentNode() const { return parentNode; }
		decltype(parentNode)& getParentNode() { return parentNode; }
		void setParentNode(const decltype(parentNode)& newValue) { parentNode = newValue; }
		
		const decltype(position)& getPosition() const { return position; }
		decltype(position)& getPosition() { return position; }
		void setPosition(const decltype(position)& newValue) { position = newValue; }
		
		const decltype(size)& getSize() const { return size; }
		decltype(size)& getSize() { return size; }
		void setSize(const decltype(size)& newValue) { size = newValue; }
		
/*GENERATED_CODE_END*/
	};
}