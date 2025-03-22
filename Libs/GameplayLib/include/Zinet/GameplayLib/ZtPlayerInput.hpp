#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Window/ZtKeyboardEvent.hpp"

namespace zt::gameplay_lib
{
	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API PlayerInput : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("PlayerInput");

	public:

	protected:

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<PlayerInput>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<PlayerInput>(); *result = *this; return result; }
		
		PlayerInput() = default;
		PlayerInput(const PlayerInput& other) = default;
		PlayerInput(PlayerInput&& other) = default;
		~PlayerInput() noexcept = default;
		
		PlayerInput& operator = (const PlayerInput& other) = default;
		PlayerInput& operator = (PlayerInput&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "PlayerInput"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}