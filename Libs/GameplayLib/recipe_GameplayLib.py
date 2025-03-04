from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetGameplayLib"
generator.targetPrettyName = "Zinet Gameplay Lib"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ZinetOpenGLRenderer
    ZinetSoftwareRenderer
    """
project_generator.add_generator(generator)