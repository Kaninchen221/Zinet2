from pathlib import Path
from zinet_reflector.parser import *
from zinet_reflector.reflector import Reflector
from zinet_utilities.paths import find_tools_folder


class TestParser:

    def test_parser(self):
        print('')
        path_to_main = find_tools_folder() / r"tests/zinet_reflector/test_files/include/zinet/lib_name/main.cpp"
        parser = Parser()
        parse_result = parser.parse(path_to_main)
        print("Parser result:")
        print_parser_result(parse_result)
        assert parse_result.children
