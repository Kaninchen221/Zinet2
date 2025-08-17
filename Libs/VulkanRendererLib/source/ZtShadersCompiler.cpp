#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtFile.hpp"

#include <glslang/SPIRV/GlslangToSpv.h>

namespace zt::vulkan_renderer
{

	static const TBuiltInResource DefaultTBuiltInResource = {
		.maxLights = 32,
		.maxClipPlanes = 6,
		.maxTextureUnits = 32,
		.maxTextureCoords = 32,
		.maxVertexAttribs = 64,
		.maxVertexUniformComponents = 4096,
		.maxVaryingFloats = 64,
		.maxVertexTextureImageUnits = 32,
		.maxCombinedTextureImageUnits = 80,
		.maxTextureImageUnits = 32,
		.maxFragmentUniformComponents = 4096,
		.maxDrawBuffers = 32,
		.maxVertexUniformVectors = 128,
		.maxVaryingVectors = 8,
		.maxFragmentUniformVectors = 16,
		.maxVertexOutputVectors = 16,
		.maxFragmentInputVectors = 15,
		.minProgramTexelOffset = -8,
		.maxProgramTexelOffset = 7,
		.maxClipDistances = 8,
		.maxComputeWorkGroupCountX = 65535,
		.maxComputeWorkGroupCountY = 65535,
		.maxComputeWorkGroupCountZ = 65535,
		.maxComputeWorkGroupSizeX = 1024,
		.maxComputeWorkGroupSizeY = 1024,
		.maxComputeWorkGroupSizeZ = 64,
		.maxComputeUniformComponents = 1024,
		.maxComputeTextureImageUnits = 16,
		.maxComputeImageUniforms = 8,
		.maxComputeAtomicCounters = 8,
		.maxComputeAtomicCounterBuffers = 1,
		.maxVaryingComponents = 60,
		.maxVertexOutputComponents = 64,
		.maxGeometryInputComponents = 64,
		.maxGeometryOutputComponents = 128,
		.maxFragmentInputComponents = 128,
		.maxImageUnits = 8,
		.maxCombinedImageUnitsAndFragmentOutputs = 8,
		.maxCombinedShaderOutputResources = 8,
		.maxImageSamples = 0,
		.maxVertexImageUniforms = 0,
		.maxTessControlImageUniforms = 0,
		.maxTessEvaluationImageUniforms = 0,
		.maxGeometryImageUniforms = 0,
		.maxFragmentImageUniforms = 8,
		.maxCombinedImageUniforms = 8,
		.maxGeometryTextureImageUnits = 16,
		.maxGeometryOutputVertices = 256,
		.maxGeometryTotalOutputComponents = 1024,
		.maxGeometryUniformComponents = 1024,
		.maxGeometryVaryingComponents = 64,
		.maxTessControlInputComponents = 128,
		.maxTessControlOutputComponents = 128,
		.maxTessControlTextureImageUnits = 16,
		.maxTessControlUniformComponents = 1024,
		.maxTessControlTotalOutputComponents = 4096,
		.maxTessEvaluationInputComponents = 128,
		.maxTessEvaluationOutputComponents = 128,
		.maxTessEvaluationTextureImageUnits = 16,
		.maxTessEvaluationUniformComponents = 1024,
		.maxTessPatchComponents = 120,
		.maxPatchVertices = 32,
		.maxTessGenLevel = 64,
		.maxViewports = 16,
		.maxVertexAtomicCounters = 0,
		.maxTessControlAtomicCounters = 0,
		.maxTessEvaluationAtomicCounters = 0,
		.maxGeometryAtomicCounters = 0,
		.maxFragmentAtomicCounters = 8,
		.maxCombinedAtomicCounters = 8,
		.maxAtomicCounterBindings = 1,
		.maxVertexAtomicCounterBuffers = 0,
		.maxTessControlAtomicCounterBuffers = 0,
		.maxTessEvaluationAtomicCounterBuffers = 0,
		.maxGeometryAtomicCounterBuffers = 0,
		.maxFragmentAtomicCounterBuffers = 1,
		.maxCombinedAtomicCounterBuffers = 1,
		.maxAtomicCounterBufferSize = 16384,
		.maxTransformFeedbackBuffers = 4,
		.maxTransformFeedbackInterleavedComponents = 64,
		.maxCullDistances = 8,
		.maxCombinedClipAndCullDistances = 8,
		.maxSamples = 4,
		.limits = {
			.nonInductiveForLoops = 1,
			.whileLoops = 1,
			.doWhileLoops = 1,
			.generalUniformIndexing = 1,
			.generalAttributeMatrixVectorIndexing = 1,
			.generalVaryingIndexing = 1,
			.generalSamplerIndexing = 1,
			.generalVariableIndexing = 1,
			.generalConstantMatrixVectorIndexing = 1,
		}
	};

	ShadersCompiler::CompileResult ShadersCompiler::compileFromFile(const std::filesystem::path& filePath, ShaderType shaderType) const
	{
		core::File file;
		file.open(filePath, core::FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file: {}", filePath.string());
			return {};
		}

		const auto sourceCode = file.readAll();
		const auto sourceFileName = filePath.filename().generic_string();

		return compileFromString(sourceCode, shaderType, sourceFileName);
	}

	ShadersCompiler::CompileResult ShadersCompiler::compileFromString(const std::string& source, ShaderType shaderType, [[maybe_unused]] const std::string& fileName) const
	{
		glslang::InitializeProcess();

		EShLanguage stage = ShaderTypeToESgLanguage(shaderType);
		glslang::TShader shader(stage);
		const char* sourceCStr = source.c_str();
		shader.setStrings(&sourceCStr, 1);

		const EShMessages messages = static_cast<EShMessages>(EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules);

		if (!shader.parse(&DefaultTBuiltInResource, 450, false, messages))
		{
			Logger->error("Shader parsing failed: {}", shader.getInfoLog());
			return {};
		}

		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(messages))
		{
			Logger->error("Shader linking failed: {}", program.getInfoLog());
			return {};
		}

		std::vector<uint32_t> spirv;
		glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

		glslang::FinalizeProcess();

		return spirv;
	}
}