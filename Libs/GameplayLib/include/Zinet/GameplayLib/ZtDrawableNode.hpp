#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API DrawableNode : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("DrawableNode");

	public:

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2f size{ 32, 32 };

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<DrawableNode>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<DrawableNode>(); *result = *this; return result; }
		
		DrawableNode() = default;
		DrawableNode(const DrawableNode& other) = default;
		DrawableNode(DrawableNode&& other) = default;
		~DrawableNode() noexcept = default;
		
		DrawableNode& operator = (const DrawableNode& other) = default;
		DrawableNode& operator = (DrawableNode&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "DrawableNode"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(size)& getSize() const { return size; }
		decltype(size)& getSize() { return size; }
		void setSize(const decltype(size)& newValue) { size = newValue; }
		
/*GENERATED_CODE_END*/
	};
}