from zinet_generator.cmakelists_generator_root import CMakelistsGeneratorRoot

root = CMakelistsGeneratorRoot()
root.cmakeMinimumVersion = "3.20"
root.projectDescription = "Game Engine"
root.projectName = "Zinet2"
root.projectVersion = "0.0.1"
root.subdirectories = ["CoreLib", "VulkanRendererLib", "WindowLib", "MathLib", "GameplayLib", "TestUtilsLib", "Sandbox", "ThirdParty/ImGuiLib", "ThirdParty/ImPlotLib"]
root.globalCompileOptionsMSVC = "/W4 /WX /external:W0 /external:anglebrackets /MP"
root.globalCompileOptionsGNU = "-Wall -Wextra -w -Werror -pedantic -pedantic-errors"
root.globalCompileOptionsForAddressSanitizer = "/fsanitize=address /RTCu /experimental:module-"
root.globalCompileDefinitions = "ZINET_STATIC ZINET_ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\""
project_generator.add_generator(root)

from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

CMakeListsGeneratorLibrary.libraryType = "STATIC"