from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from zinet_generator.cmakelists_generator_library import CMakeListsGeneratorLibrary
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCMakeListsGeneratorLibrary():
    
    def test_inheritance(self):
        assert issubclass(CMakeListsGeneratorLibrary, CMakeListsGeneratorTarget)

    def test_properties(self):
        assert self.generator_library.libraryType == "STATIC"
        assert self.generator_library.shouldAddTests == "TRUE"
        assert self.generator_library.testsSubfoldersPrefix == "tests_"
        expected_template_path = CMakeListsGenerator.get_templates_folder() / "CMakeListsLibraryTemplate.txt"
        assert self.generator_library.templatePath == expected_template_path
        assert self.generator_library.templatePath.exists()

    def test_prepare_arguments(self):
        test_files_path = find_tools_folder() / r"tests/zinet_generator/test_files/expected_library_cmakelists.txt"
        assert test_files_path.exists()
        self.generator_library.fileLocation = test_files_path

        expected_tests_subfolders_arguments = ["add_subdirectory(tests_system)\n\t", "add_subdirectory(tests_unit)\n\t"]

        arguments = self.generator_library.prepare_arguments()
        assert type(arguments) is SafeDict
        assert arguments['argument_library_type'] == self.generator_library.libraryType
        assert arguments['argument_should_add_tests'] == self.generator_library.shouldAddTests
        assert (arguments['argument_tests_subfolders'] == expected_tests_subfolders_arguments[0] + expected_tests_subfolders_arguments[1] or
                arguments['argument_tests_subfolders'] == expected_tests_subfolders_arguments[1] + expected_tests_subfolders_arguments[0])

    def test_generate_cmake(self):
        file_location = find_tools_folder() / r"tests/zinet_generator/test_files/expected_library_cmakelists.txt"
        assert file_location.exists()
        self.generator_library.fileLocation = file_location
        arguments = self.generator_library.prepare_arguments()

        cmakelists = self.generator_library.generate_cmakelists(arguments)
        expected_cmake_lists = open(find_tools_folder() / r"tests/zinet_generator/test_files"
                                                          r"/expected_library_cmakelists.txt").read()
        expected_cmake_lists = expected_cmake_lists.format_map(SafeDict(argument_absolute_path=find_tools_folder().parent))
        expected_cmake_lists = expected_cmake_lists.replace("\\\\", "/")
        expected_cmake_lists = expected_cmake_lists.replace("\\", "/")

        assert len(cmakelists) == len(expected_cmake_lists)

    generator_library = CMakeListsGeneratorLibrary()