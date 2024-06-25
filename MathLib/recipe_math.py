from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetMath"
generator.targetPrettyName = "Zinet Math"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_GLM}
    """
generator.linkLibraries = """
    ZinetCore
    ${CONAN_LIBS_GLM}
    """
project_generator.add_generator(generator)