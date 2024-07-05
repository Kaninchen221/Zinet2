from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "Glad"
generator.targetPrettyName = "Glad Lib"
generator.headersExtension = "hpp"
generator.sourcesExtension = "cpp"
generator.compileDefinitions = """
    GLAD_API_CALL_EXPORT
    GLAD_API_CALL_EXPORT_BUILD
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    """
project_generator.add_generator(generator)