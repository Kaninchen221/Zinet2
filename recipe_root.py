from zinet_generator.cmakelists_generator_root import CMakelistsGeneratorRoot

root = CMakelistsGeneratorRoot()
root.cmakeMinimumVersion = "3.20"
root.projectDescription = "Game Engine"
root.projectName = "Zinet2"
root.projectVersion = "0.0.1"
root.subdirectories = ["CoreLib", "RHILib", "WindowLib", "MathLib", "ThirdParty/ImGuiLib", "ThirdParty/Glad"]
root.globalCompileOptions = "/W4 /WX /external:W0 /external:anglebrackets /MP"
root.globalCompileOptionsForAddressSanitizer = "/fsanitize=address /RTCu /experimental:module-"
root.globalCompileDefinitions = "ZINET_WINDOWS _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING ZINET_USE_OPENGL"
project_generator.add_generator(root)

from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary

CMakeListsGeneratorLibrary.libraryType = "SHARED"