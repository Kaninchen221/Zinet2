from pathlib import Path

from zinet_reflector.code_generators.code_generator_constructors import CodeGeneratorConstructors
from zinet_reflector.code_generators.code_generator_getter_setter import CodeGeneratorGetterSetter
from zinet_reflector.code_generators.code_generator_operators import CodeGeneratorOperators
from zinet_reflector.code_injector import CodeInjector
from zinet_reflector.parser import *
from zinet_reflector.assignor import *
from zinet_reflector.tokens_finder import *
from zinet_reflector.code_generator import *
from zinet_utilities.paths import find_tools_folder


class TestCodeInjector:

    def test_inject_code(self):
        path_to_main = find_tools_folder() / r"tests\zinet_reflector\test_files\include\zinet\lib_name\main.cpp"
        parser = Parser()
        parse_result = parser.parse(path_to_main)

        assignor = Assignor()
        assignor.assign(parse_result)

        tokens_finder = TokensFinder()
        tokens_finder.find_tokens(parse_result)

        #print("Parse result after assigning:")
        #print_parser_result(parser_result)

        code_generator = CodeGenerator()
        code_generator.instructions.append(CodeGeneratorConstructors())
        code_generator.instructions.append(CodeGeneratorOperators())
        code_generator.instructions.append(CodeGeneratorGetterSetter())

        generated_code = code_generator.generate_code(parse_result)

        #print("Generated code:")
        #print(generated_code)
        #print_generated_code(generated_code)

        code_injector = CodeInjector()
        code_injector.inject_code(generated_code)

        file = open(find_tools_folder() /
                    r"tests/zinet_reflector/test_files/include/zinet/lib_name/reflection_test_file.hpp", 'r')
        expected_file_path = str(find_tools_folder() /
                                 r"tests/zinet_reflector/test_files/include/zinet/lib_name"
                                 r"/reflection_test_file_expected.h")
        expected_file = open(expected_file_path, 'r')
        file_content = file.read()
        expected_file_content = expected_file.read()
        assert len(file_content) > len(expected_file_content)
