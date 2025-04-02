#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"
#include "Zinet/GameplayLib/ZtCamera.hpp"

#include "Zinet/GameplayLib/Systems/ZtTickableSystem.hpp"
#include "Zinet/GameplayLib/Systems/ZtDrawableSystem.hpp"
#include "Zinet/GameplayLib/Systems/ZtWindowEventsSystem.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClock.hpp"

#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay_lib
{
	namespace oglr = opengl_renderer;
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API GameplayLoop : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("GameplayLoop");
		inline static std::shared_ptr<GameplayLoop> ActiveGameplayLoop;

	public:

		void start();

		static auto& Get() { return ActiveGameplayLoop; }

		TickableSystem tickableSystem;
		DrawableSystem drawableSystem;
		WindowEventsSystem windowEventsSystem;

		wd::Window window;

		void setCurrentCamera(const std::shared_ptr<Camera>& camera) { drawableSystem.setCurrentCamera(camera); windowEventsSystem.setCurrentCamera(camera); }

	protected:

		oglr::OpenGLRenderer openGLRenderer;

		core::Clock loopClock;
		
		std::shared_ptr<wd::WindowEvents> windowEvents = std::make_shared<wd::WindowEvents>(window);

		core::Clock fpsCounterClock;
		std::uint32_t fpsCounter = 0u;
		std::string windowTitle;

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
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}