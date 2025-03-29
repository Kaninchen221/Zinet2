#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"
#include "Zinet/GameplayLib/ZtTileMap.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API HorizontalSlidingWorld : public Node
	{

	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("HorizontalSlidingWorld");

	public:

		bool init(std::filesystem::path contentFolderPath, std::weak_ptr<HorizontalSlidingWorld> newSelfWeakPtr);

		void tick(float deltaTime) override;

		const std::vector<std::shared_ptr<TileMap>>& getGroundSlicesNodes() { return groundSlicesNodes; }

	protected:

		void addGroundSlice();

		std::weak_ptr<HorizontalSlidingWorld> selfWeakPtr;

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2f sliceSize = Vector2f{ 48, 144 };

		std::vector<std::shared_ptr<TileMap>> groundSlicesNodes;
		sf::RenderTarget groundSliceTexture;

		ZT_REFLECT_MEMBER(ReadWrite)
		std::int32_t slicesCount = 13;

		ZT_REFLECT_MEMBER(ReadWrite)
		float slidingSpeed = .04f;

		ZT_REFLECT_MEMBER(ReadWrite)
		float deadEndX = 0.f;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<HorizontalSlidingWorld>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<HorizontalSlidingWorld>(); *result = *this; return result; }
		
		HorizontalSlidingWorld() = default;
		HorizontalSlidingWorld(const HorizontalSlidingWorld& other) = default;
		HorizontalSlidingWorld(HorizontalSlidingWorld&& other) = default;
		~HorizontalSlidingWorld() noexcept = default;
		
		HorizontalSlidingWorld& operator = (const HorizontalSlidingWorld& other) = default;
		HorizontalSlidingWorld& operator = (HorizontalSlidingWorld&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "HorizontalSlidingWorld"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(sliceSize)& getSliceSize() const { return sliceSize; }
		decltype(sliceSize)& getSliceSize() { return sliceSize; }
		void setSliceSize(const decltype(sliceSize)& newValue) { sliceSize = newValue; }
		
		const decltype(slidingSpeed)& getSlidingSpeed() const { return slidingSpeed; }
		decltype(slidingSpeed)& getSlidingSpeed() { return slidingSpeed; }
		void setSlidingSpeed(const decltype(slidingSpeed)& newValue) { slidingSpeed = newValue; }
		
		const decltype(deadEndX)& getDeadEndX() const { return deadEndX; }
		decltype(deadEndX)& getDeadEndX() { return deadEndX; }
		void setDeadEndX(const decltype(deadEndX)& newValue) { deadEndX = newValue; }
		
/*GENERATED_CODE_END*/
	};
}