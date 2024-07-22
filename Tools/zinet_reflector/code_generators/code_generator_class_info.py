from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind
from zinet_reflector.code_generators.class_info_parents_children import *


class CodeGeneratorClassInfo(CodeGeneratorInstructionBase):

    reflection_namespace = 'zt::core::reflection'

    def __init__(self):
        super().__init__()
        self.token = None
        self.parser_results = []
        self.members = []
        self.class_info_parents_children = ClassInfoParentsChildren()

    def generate_code(self, parser_result):
        if parser_result.reflection_kind == ReflectionKind.Class:
            self.parser_results.append(parser_result)
            self.class_info_parents_children.generate_code(parser_result)

        if parser_result.reflection_kind == ReflectionKind.Member:
            self.members.append(parser_result)

        return ""

    def generate_code_post(self, parser_result):
        if parser_result.reflection_kind != ReflectionKind.Class:
            return ""

        inside = ""
        outside = ""

        if get_class_name_function := self.get_class_name_function(parser_result):
            inside += get_class_name_function

        if parents_children_info := self.class_info_parents_children.generate_code_post():
            inside += parents_children_info
        #print(self.class_info_parents_children)

        class_info = (""
                      f'class ClassInfo : zt::core::ClassInfoBase'
                      '\n{'
                      '\npublic:'
                      f'\n{inside}'
                      '\n};'
                      f'{outside}\n')

        return class_info

    @staticmethod
    def get_class_name_function(class_parser_result):
        class_name = class_parser_result.get_class_name()
        return f'\n\tstatic std::string_view GetClassName() {{ return "{class_name}"; }}'
