from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetRHI"
generator.targetPrettyName = "Zinet RHI"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    GLAD_API_CALL_EXPORT
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ZinetCore
    ZinetWindow
    ZinetMath
    ImGui
    Glad
    shaderc::shaderc
    glfw
    glm::glm
    glslang::glslang
    stb::stb
    vulkan-memory-allocator::vulkan-memory-allocator
    ${Vulkan_LIBRARIES}
    spirv-tools::spirv-tools
    """
project_generator.add_generator(generator)