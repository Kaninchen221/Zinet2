#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay_lib
{
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API GameplayLoop : public core::Object
	{
	protected:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("GameplayLoop");

	public:

	protected:

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<GameplayLoop>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<GameplayLoop>(); *result = *this; return result; }
		
		GameplayLoop() = default;
		GameplayLoop(const GameplayLoop& other) = default;
		GameplayLoop(GameplayLoop&& other) = default;
		~GameplayLoop() noexcept = default;
		
		GameplayLoop& operator = (const GameplayLoop& other) = default;
		GameplayLoop& operator = (GameplayLoop&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "GameplayLoop"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{core::Object::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}