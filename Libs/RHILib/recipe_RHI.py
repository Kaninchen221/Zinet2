from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetRHI"
generator.targetPrettyName = "Zinet RHI"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_SHADERC}
    ${CONAN_INCLUDE_DIRS_GLFW}
    ${CONAN_INCLUDE_DIRS_GLM}
    ${CONAN_INCLUDE_DIRS_STB}
    ${CONAN_INCLUDE_DIRS_VULKAN-MEMORY-ALLOCATOR}
    ${Vulkan_INCLUDE_DIRS}
    ${CONAN_INCLUDE_DIRS_GLEW}
    """
generator.linkLibraries = """
    ZinetCore
    ZinetWindow
    ZinetMath
    ImGui
    ${CONAN_LIBS_SHADERC}
    ${CONAN_LIBS_GLFW}
    ${CONAN_LIBS_GLM}
    ${CONAN_LIBS_GLSLANG}
    ${CONAN_LIBS_STB}
    ${CONAN_LIBS_VULKAN-MEMORY-ALLOCATOR}
    ${Vulkan_LIBRARIES}
    ${CONAN_LIBS_SPIRV-TOOLS}
    ${CONAN_LIBS_SPIRV-HEADERS}
    ${CONAN_LIBS_OPENGL}
    ${CONAN_LIBS_GLEW}
    """
project_generator.add_generator(generator)