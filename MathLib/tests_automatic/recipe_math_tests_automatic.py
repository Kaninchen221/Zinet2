from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest

generator = CMakeListsGeneratorTest()
generator.targetName = "ZinetMathAutomaticTests"
generator.targetPrettyName = "Zinet Math Automatic Tests"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_GTEST}
    """
generator.linkLibraries =  """
    ZinetMath
    ${CONAN_LIBS_GTEST}
    """
project_generator.add_generator(generator)