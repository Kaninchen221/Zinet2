from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest

generator = CMakeListsGeneratorTest()
generator.targetName = "ZinetRHIManualTests"
generator.targetPrettyName = "Zinet RHI Manual Tests"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_GTEST}
    """
generator.linkLibraries =  """
    ZinetWindow
    ZinetRHI
    ${CONAN_LIBS_GTEST}
    """
project_generator.add_generator(generator)