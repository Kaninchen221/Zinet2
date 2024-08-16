from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCmakelistsGenerator:
    def test_generate_cmake_implementation(self):
        template_path = find_tools_folder() / r"tests/zinet_generator/test_files/template_simple.txt"
        arguments = SafeDict(arg1="1", arg2="2", arg3="3")
        cmakelists = self.cmakelistsGenerator.generate_cmake_implementation(template_path, arguments)
        assert isinstance(cmakelists, str)
        assert cmakelists == "1 3 2"

    def test_prepare_arguments(self):
        arguments = self.cmakelistsGenerator.prepare_arguments()
        assert type(arguments) is SafeDict

    def test_properties(self):
        assert issubclass(type(self.cmakelistsGenerator.fileLocation), Path)
        assert issubclass(type(self.cmakelistsGenerator.templatePath), Path)

    cmakelistsGenerator = CMakeListsGenerator()
