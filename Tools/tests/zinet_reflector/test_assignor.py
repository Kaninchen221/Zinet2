from pathlib import Path

from zinet_reflector.parser import *
from zinet_reflector.assignor import *
from zinet_utilities.paths import find_tools_folder


class TestAssignor:

    def test_assignor(self):
        path = find_tools_folder() / r"tests/zinet_reflector/test_files/include/zinet/lib_name/reflection_test_file.hpp"
        parser = Parser()
        parser_results = parser.parse(path)

        assignor = Assignor()
        assignor.assign(parser_results)
        print("Parse result after assigning:")
        print_parser_result(parser_results)
