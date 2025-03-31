#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtCamera.hpp"

#include "Zinet/GameplayLib/Systems/ZtSystem.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay_lib
{
	// TODO: Optimize sf::DrawInfo
	// Draw info should be stored in objects that want to be draw and they should decide if they need update
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API DrawableSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("DrawableSystem");

	public:

		void tick(float deltaTime) override;

		void addNode(const std::weak_ptr<Node>& node);

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::shared_ptr<Camera> currentCamera;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<std::weak_ptr<Node>> nodes;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<sf::DrawInfo> nodesDrawInfos;

		sf::SoftwareRenderer softwareRenderer;

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<DrawableSystem>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<DrawableSystem>(); *result = *this; return result; }
		
		DrawableSystem() = default;
		DrawableSystem(const DrawableSystem& other) = default;
		DrawableSystem(DrawableSystem&& other) = default;
		~DrawableSystem() noexcept = default;
		
		DrawableSystem& operator = (const DrawableSystem& other) = default;
		DrawableSystem& operator = (DrawableSystem&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "DrawableSystem"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(currentCamera)& getCurrentCamera() const { return currentCamera; }
		decltype(currentCamera)& getCurrentCamera() { return currentCamera; }
		void setCurrentCamera(const decltype(currentCamera)& newValue) { currentCamera = newValue; }
		
		const decltype(nodes)& getNodes() const { return nodes; }
		
/*GENERATED_CODE_END*/
	};
}