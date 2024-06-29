from zinet_reflector.parser_result import *


class CodeGeneratorInstructionBase:
    def __init__(self):
        pass

    def generate_code(self, parser_result):
        return None

    def generate_code_post(self, file_path):
        return None


class CodeGenerator:
    def __init__(self):
        self.instructions = []

    def generate_code(self, parser_result):
        generated_code = {}
        self._generate_code_internal(parser_result, generated_code)
        self._generate_code_post(generated_code)
        return generated_code

    def _generate_code_internal(self, parser_result, generated_code):
        for child_parser_result in parser_result.children:
            for instruction in self.instructions:
                instruction_generated_code = instruction.generate_code(child_parser_result)

                if parser_result.cursor.location.file is None:
                    continue

                if not instruction_generated_code:
                    continue

                key = parser_result.cursor.location.file.name
                if key not in generated_code:
                    generated_code[key] = []
                generated_code[key].append(instruction_generated_code)

            self._generate_code_internal(child_parser_result, generated_code)

    def _generate_code_post(self, generated_code):
        for file_path, code in generated_code.items():
            for instruction in self.instructions:
                generated_code_post = instruction.generate_code_post(file_path)
                if generated_code_post:
                    code.append(generated_code_post)


def print_generated_code(generated_code):
    print("Generated code:")
    for file_name, generated_code_strings in generated_code.items():
        print(f"{file_name}:")
        for generated_code_str in generated_code_strings:
            print(f"{generated_code_str}")
