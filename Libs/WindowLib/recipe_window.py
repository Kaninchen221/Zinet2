from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetWindow"
generator.targetPrettyName = "Zinet Window"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ZinetCore
    ZinetMath
    ${Vulkan_LIBRARIES}
    ${glslang_LIBRARIES}
    ${shaderc_LIBRARIES}
    ${SPIRV-Tools_LIBRARIES}
    ${vulkan-memory-allocator_LIBRARIES}
    spdlog::spdlog
    fmt::fmt
    glfw
    glm::glm
    """
project_generator.add_generator(generator)