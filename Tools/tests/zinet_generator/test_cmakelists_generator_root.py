from zinet_generator.cmakelists_generator_root import CMakelistsGeneratorRoot
from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCmakelistsGeneratorRoot:
    def test_inheritance(self):
        assert issubclass(CMakelistsGeneratorRoot, CMakeListsGenerator)

    def test_properties(self):
        expected_template_path = CMakeListsGenerator.get_templates_folder() / "CMakeListsRootTemplate.txt"
        assert self.generator_root.templatePath == expected_template_path
        assert self.generator_root.templatePath.exists()

    def test_prepare_arguments(self):
        self.prepare_arguments()
        arguments = self.generator_root.prepare_arguments()
        
        assert type(arguments) is SafeDict
        assert arguments['argument_cmake_minimum_version'] == self.generator_root.cmakeMinimumVersion
        assert arguments['argument_project_name'] == self.generator_root.projectName
        assert arguments['argument_project_version'] == self.generator_root.projectVersion
        assert arguments['argument_project_description'] == self.generator_root.projectDescription
        assert arguments['argument_global_compile_options_gnu'] == self.generator_root.globalCompileOptionsGNU
        assert arguments['argument_global_compile_options_msvc'] == self.generator_root.globalCompileOptionsMSVC
        assert arguments['argument_global_compile_definitions'] == self.generator_root.globalCompileDefinitions
        assert (arguments['argument_subdirectories'] ==
                f"add_subdirectory({self.generator_root.libsPath}CoreLib)\n"
                f"add_subdirectory({self.generator_root.libsPath}RHILib)\n"
                f"add_subdirectory({self.generator_root.libsPath}WindowLib)\n"
                f"add_subdirectory({self.generator_root.libsPath}MathLib)\n"
                f"add_subdirectory({self.generator_root.libsPath}ThirdParty/ImGuiLib)\n"
                f"add_subdirectory({self.generator_root.libsPath}ThirdParty/Glad)\n")

    def test_generate_cmakelists(self):
        self.prepare_arguments()
        arguments = self.generator_root.prepare_arguments()
        cmakelists = self.generator_root.generate_cmakelists(arguments)
        expected_cmake_lists_path = find_tools_folder() / r"tests/zinet_generator/test_files/expected_root.txt"
        expected_cmake_lists = open(expected_cmake_lists_path).read()

        assert cmakelists == expected_cmake_lists

    def prepare_arguments(self):
        self.generator_root.cmakeMinimumVersion = "1.0.0"
        self.generator_root.projectName = "project name"
        self.generator_root.projectVersion = "project version"
        self.generator_root.projectDescription = "description"
        self.generator_root.globalCompileOptionsGNU = "-w -Werror"
        self.generator_root.globalCompileOptionsMSVC = "/W4 /WX"
        self.generator_root.globalCompileOptionsForAddressSanitizer = "/AddressSanitizer"
        self.generator_root.globalCompileDefinitions = "opt1 opt2 opt3"
        self.generator_root.subdirectories = ["CoreLib", "RHILib", "WindowLib", "MathLib", "ThirdParty/ImGuiLib", "ThirdParty/Glad"]

    generator_root = CMakelistsGeneratorRoot()
