from pathlib import Path

from zinet_reflector.code_generators.code_generator_class_info import CodeGeneratorClassInfo
from zinet_reflector.code_generators.code_generator_constructors import CodeGeneratorConstructors
from zinet_reflector.code_generators.code_generator_getter_setter import CodeGeneratorGetterSetter
from zinet_reflector.code_generators.code_generator_operators import CodeGeneratorOperators
from zinet_reflector.parser import *
from zinet_reflector.assignor import *
from zinet_reflector.tokens_finder import *
from zinet_reflector.code_generator import *
from zinet_utilities.paths import find_tools_folder


class TestCodeGenerator:

    def test_generate_code(self):
        path = find_tools_folder() / (r"tests/zinet_reflector/test_files/include/zinet/lib_name"
                                      r"/reflection_test_file_with_macros.hpp")
        parser = Parser()
        parse_result = parser.parse(path)

        assignor = Assignor()
        assignor.assign(parse_result)

        tokens_finder = TokensFinder()
        tokens_finder.find_tokens(parse_result)

        #print("Parse result after assigning:")
        print_parser_result(parse_result)

        code_generator = CodeGenerator()
        code_generator.instructions.append(CodeGeneratorConstructors())
        code_generator.instructions.append(CodeGeneratorOperators())
        code_generator.instructions.append(CodeGeneratorGetterSetter())
        code_generator.instructions.append(CodeGeneratorClassInfo())

        generated_code = code_generator.generate_code(parse_result)

        print("Generated code:")
        print(generated_code)
        print_generated_code(generated_code)

        assert generated_code
