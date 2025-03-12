#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtDrawable.hpp"
#include "Zinet/GameplayLib/ZtTickable.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"
#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	struct ZINET_GAMEPLAY_LIB_API FlipbookFrame
	{
		sf::RenderTarget texture;
		RectF textureRegion // Normalized coords
		{
			{ 0.f, 0.f },
			{ 1.f, 1.f }
		};
		float time = 0.0f;
	};

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API Flipbook : public core::Object, public Drawable, public Tickable
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Flipbook");

	public:

		void addFrame(const FlipbookFrame& frame);

		void tick(float deltaTime) override;

		sf::DrawInfo getDrawInfo() const override;

	protected:

		float updateFrame(float deltaTime);

		ZT_REFLECT_MEMBER(ReadOnly)
		std::vector<FlipbookFrame> frames;

		ZT_REFLECT_MEMBER(ReadOnly)
		size_t currentFrameIndex = 0u;

		float elapsedTime = 0.f;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Flipbook>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Flipbook>(); *result = *this; return result; }
		
		Flipbook() = default;
		Flipbook(const Flipbook& other) = default;
		Flipbook(Flipbook&& other) = default;
		~Flipbook() noexcept = default;
		
		Flipbook& operator = (const Flipbook& other) = default;
		Flipbook& operator = (Flipbook&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Flipbook"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{core::Object::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(frames)& getFrames() const { return frames; }
		
		const decltype(currentFrameIndex)& getCurrentFrameIndex() const { return currentFrameIndex; }
		
/*GENERATED_CODE_END*/
	};
}