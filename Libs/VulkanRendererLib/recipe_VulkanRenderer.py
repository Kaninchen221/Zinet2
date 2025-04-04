from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetVulkanRenderer"
generator.targetPrettyName = "Zinet Vulkan Renderer"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ZinetCore
    ZinetMath
    ZinetWindow
    """
project_generator.add_generator(generator)