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

	protected:

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
		
		
/*GENERATED_CODE_END*/
	};
}