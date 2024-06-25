from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorOperators(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None

    def generate_code(self, parser_result):
        if parser_result.reflection_kind == self.reflection_kind:
            class_name = parser_result.get_class_name()
            return f"""
{class_name}& operator = (const {class_name}& other) = default;
{class_name}& operator = ({class_name}&& other) = default;"""
