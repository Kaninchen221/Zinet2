from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

generator = CMakeListsGeneratorLibrary()
generator.targetName = "ZinetCore"
generator.targetPrettyName = "Zinet Core"
generator.compileDefinitions = """
    ZINET_CURRENT_PROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
    ZINET_LIB
    """
generator.includeDirectories = """
    """
generator.linkLibraries = """
    spdlog::spdlog
    plf_colony::plf_colony
    nlohmann_json::nlohmann_json
    glm::glm
    spdlog::spdlog
    """
project_generator.add_generator(generator)