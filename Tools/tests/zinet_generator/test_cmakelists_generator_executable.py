from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from zinet_generator.cmakelists_generator_executable import CMakeListsGeneratorExecutable
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCMakeListsGeneratorExecutable():
    
    def test_inheritance(self):
        assert issubclass(CMakeListsGeneratorExecutable, CMakeListsGeneratorTarget)

    def test_properties(self):
        expected_template_path = (find_tools_folder() / r"zinet_generator/templates/CMakeListsExecutableTemplate.txt")
        assert self.generator_executable.templatePath == expected_template_path
        assert self.generator_executable.templatePath.exists()

    def test_generate_cmake(self):
        # Fake file path
        file_location = find_tools_folder() / r"tests/zinet_generator/test_files/expected_executable_cmakelists.txt"
        self.generator_executable.fileLocation = file_location
        arguments = self.generator_executable.prepare_arguments()

        cmakelists = self.generator_executable.generate_cmakelists(arguments)
        expected_cmake_lists = open(file_location).read()
        expected_cmake_lists = expected_cmake_lists.format_map(SafeDict(argument_absolute_path=find_tools_folder().parent))
        expected_cmake_lists = expected_cmake_lists.replace("\\\\", "/")
        expected_cmake_lists = expected_cmake_lists.replace("\\", "/")

        assert cmakelists == expected_cmake_lists

    generator_executable = CMakeListsGeneratorExecutable()