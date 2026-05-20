from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetNativeLang"
generator.targetPrettyName = "Zinet Native Lang"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ZinetCore
    ZinetMath
    """
project_generator.add_generator(generator)