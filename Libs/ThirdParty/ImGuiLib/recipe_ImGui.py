from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ImGui"
generator.targetPrettyName = "Imgui"
generator.compileDefinitions = """
    ZINET_LIB
    """
generator.includeDirectories = """
    ${CONAN_INCLUDE_DIRS_GLFW}
    ${Vulkan_INCLUDE_DIRS}
    """
generator.linkLibraries = """
    ${CONAN_LIBS_GLFW}
    ${Vulkan_LIBRARIES}
    """
project_generator.add_generator(generator)