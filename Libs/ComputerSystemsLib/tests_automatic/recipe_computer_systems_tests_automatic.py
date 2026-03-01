from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest

generator = CMakeListsGeneratorTest()
generator.targetName = "ZinetComputerSystemsAutomaticTests"
generator.targetPrettyName = "Zinet Computer Systems Automatic Tests"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    """
generator.includeDirectories = """
    """
generator.linkLibraries =  """
    ZinetComputerSystems
    ZinetTestUtils
    gtest::gtest
    """
project_generator.add_generator(generator)