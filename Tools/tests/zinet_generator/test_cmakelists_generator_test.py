from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from zinet_generator.cmakelists_generator_test import CMakeListsGeneratorTest
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCMakeListsGeneratorTest():
    
    def test_inheritance(self):
        assert issubclass(CMakeListsGeneratorTest, CMakeListsGeneratorTarget)

    def test_properties(self):
        expected_template_path = CMakeListsGenerator.get_templates_folder() / "CMakeListsTestTemplate.txt"
        assert self.generatorTest.templatePath == expected_template_path
        assert self.generatorTest.templatePath.exists()

    def test_generate_cmake(self):
        # Fake file path
        file_location = find_tools_folder() / "tests/zinet_generator/test_files/expected_test_cmakelists.txt"
        self.generatorTest.fileLocation = file_location
        arguments = self.generatorTest.prepare_arguments()

        cmakelists = self.generatorTest.generate_cmakelists(arguments)
        expected_cmake_lists = open(find_tools_folder() / "tests/zinet_generator/test_files/expected_test_cmakelists"
                                                          ".txt").read()
        expected_cmake_lists = expected_cmake_lists.format_map(SafeDict(argument_absolute_path=find_tools_folder().parent))
        expected_cmake_lists = expected_cmake_lists.replace("\\\\", "/")
        expected_cmake_lists = expected_cmake_lists.replace("\\", "/")

        assert cmakelists == expected_cmake_lists

    generatorTest = CMakeListsGeneratorTest()

