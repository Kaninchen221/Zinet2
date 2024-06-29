from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorClassInfo(CodeGeneratorInstructionBase):

    reflection_namespace = 'zt::core::reflection'

    def __init__(self):
        super().__init__()
        self.token = None
        self.members = []
        self.parser_results = []

    def generate_code(self, parser_result):
        result = None
        if parser_result.reflection_kind == ReflectionKind.Class:
            self.parser_results.append(parser_result)
            result = " "

        if parser_result.reflection_kind == ReflectionKind.Member:
            self.members.append(parser_result)
            result = " "

        return result

    def generate_code_post(self, file_path):
        class_parser_result = None
        for parser_result in self.parser_results:
            if parser_result.get_cursor_file_path() == file_path:
                class_parser_result = parser_result
                break

        if class_parser_result:
            inside = ""
            outside = ""

            if get_class_name_function := self.get_class_name_function(class_parser_result):
                inside += get_class_name_function

            class_info = (""
                          f'\nclass ClassInfo'
                          '\n{'
                          '\npublic:'
                          f'\n{inside}'
                          '\n};'
                          f'{outside}')

            return class_info

    @staticmethod
    def get_class_name_function(class_parser_result):
        class_name = class_parser_result.get_class_name()
        return f'\n\tstatic std::string_view GetClassName() {{ return "{class_name}"; }}'
