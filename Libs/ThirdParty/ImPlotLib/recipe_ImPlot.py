from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ImPlot"
generator.targetPrettyName = "ImPlot"
generator.compileDefinitions = """
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    ImGui
    """
project_generator.add_generator(generator)