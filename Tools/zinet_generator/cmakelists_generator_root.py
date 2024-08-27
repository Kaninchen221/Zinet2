from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.safe_dict import SafeDict


class CMakelistsGeneratorRoot(CMakeListsGenerator):

    def __init__(self) -> None:
        super().__init__()
        self.templatePath = CMakeListsGenerator.get_templates_folder() / "CMakeListsRootTemplate.txt"
        self.libsPath = "Libs/"

    def prepare_arguments(self):
        subdirectories_arg = self.create_argument_subdirectories()
        arguments = SafeDict(
            argument_cmake_minimum_version = self.cmakeMinimumVersion,
            argument_project_name = self.projectName,
            argument_project_version = self.projectVersion,
            argument_project_description = self.projectDescription,
            argument_global_compile_options_msvc = self.globalCompileOptionsMSVC,
            argument_global_compile_options_gnu = self.globalCompileOptionsGNU,
            argument_global_compile_definitions = self.globalCompileDefinitions,
            argument_subdirectories = subdirectories_arg
            )
        
        if hasattr(self.cmdArgs, 'AddressSanitizer') and self.cmdArgs.AddressSanitizer == "true":
            arguments["argument_global_compile_options"] += " " + self.globalCompileOptionsForAddressSanitizer

        return arguments

    def create_argument_subdirectories(self):
        argument = ""
        for subdirectory in self.subdirectories:
            argument += f"add_subdirectory({self.libsPath}{subdirectory})\n"
        return argument

    cmakeMinimumVersion = ''
    projectName = ''
    projectVersion = ''
    projectDescription = ''
    globalCompileOptionsMSVC = ''
    globalCompileOptionsGNU = ''
    globalCompileOptionsForAddressSanitizer = ''
    globalCompileDefinitions = ''
    subdirectories = ''
