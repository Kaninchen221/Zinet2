#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	struct DrawInfo;

	ZT_REFLECT_CLASS()
	class ZINET_VULKAN_RENDERER_API VulkanRenderer : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer");

	public:

		bool init(wd::Window& window) noexcept;

		void deinit() noexcept;

		bool beginFrame() noexcept;

		bool createPipeline(DrawInfo& drawInfo) noexcept;
		
		void draw(const DrawInfo& drawInfo) noexcept;

		bool submit() noexcept;

		bool endFrame() noexcept;

		const auto& getRendererContext() const noexcept { return rendererContext; }
		auto& getRendererContext() noexcept { return rendererContext; }

		const auto& getGraphicsPipeline() const noexcept { return graphicsPipeline; }
		auto& getGraphicsPipeline() noexcept { return graphicsPipeline; }

	protected:

		RendererContext rendererContext;
		GraphicsPipeline graphicsPipeline;

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<VulkanRenderer>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<VulkanRenderer>(); *result = *this; return result; }
		
		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) = default;
		~VulkanRenderer() noexcept = default;
		
		VulkanRenderer& operator = (const VulkanRenderer& other) = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "VulkanRenderer"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}