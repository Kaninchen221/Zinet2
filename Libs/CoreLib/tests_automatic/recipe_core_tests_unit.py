from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest

generator = CMakeListsGeneratorTest()
generator.targetName = "ZinetCoreAutomaticTests"
generator.targetPrettyName = "Zinet Core Automatic Tests"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    """
generator.includeDirectories = """
    """
generator.linkLibraries =  """
    ZinetCore
    gtest::gtest
    """
project_generator.add_generator(generator)