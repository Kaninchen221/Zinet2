from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest

generator = CMakeListsGeneratorTest()
generator.targetName = "ZinetOpenGLRendererManualTests"
generator.targetPrettyName = "Zinet OpenGL Renderer Manual Tests"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_GTEST}
    """
generator.linkLibraries =  """
    ZinetOpenGLRenderer
    ZinetSoftwareRenderer
    ZinetWindow
    gtest::gtest
    """
project_generator.add_generator(generator)