from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetMath"
generator.targetPrettyName = "Zinet Math"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    ${glm_INCLUDE_DIRS}
    """
generator.linkLibraries = """
    ZinetCore
    ${glm_LIBRARIES}
    """
project_generator.add_generator(generator)