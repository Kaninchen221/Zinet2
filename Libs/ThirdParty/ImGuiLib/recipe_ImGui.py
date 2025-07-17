from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ImGui"
generator.targetPrettyName = "Imgui"
generator.compileDefinitions = """
    ZINET_LIB
    """
generator.includeDirectories = """
    ${VulkanHeaders_INCLUDE_DIRS}
    """
generator.linkLibraries = """
    glfw
    ${Vulkan_LIBRARIES}
    """
project_generator.add_generator(generator)