#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"

#include "Zinet/GameplayLib/Systems/ZtSystem.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"


namespace zt::gameplay_lib
{
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API TickableSystem : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("TickableSystem");

	public:

		void tick(float deltaTime) override;

		void addNode(const std::weak_ptr<Node>& node) { nodes.push_back(node); }

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<std::weak_ptr<Node>> nodes;

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<TickableSystem>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<TickableSystem>(); *result = *this; return result; }
		
		TickableSystem() = default;
		TickableSystem(const TickableSystem& other) = default;
		TickableSystem(TickableSystem&& other) = default;
		~TickableSystem() noexcept = default;
		
		TickableSystem& operator = (const TickableSystem& other) = default;
		TickableSystem& operator = (TickableSystem&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "TickableSystem"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(nodes)& getNodes() const { return nodes; }
		
/*GENERATED_CODE_END*/
	};
}