from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetWindow"
generator.targetPrettyName = "Zinet Window"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_SPDLOG}
    ${CONAN_INCLUDE_DIRS_FMT}
    ${CONAN_INCLUDE_DIRS_GLFW}
    ${CONAN_INCLUDE_DIRS_GLM}
    """
generator.linkLibraries = """
    ZinetCore
    ZinetMath
    ${CONAN_LIBS_SPDLOG}
    ${CONAN_LIBS_FMT}
    ${CONAN_LIBS_GLFW}
    ${CONAN_LIBS_GLM}
    """
project_generator.add_generator(generator)