from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorObjectBaseClass(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None
        self.mute_token = "NO_TEST_BASE_CLASS_OBJECT"

    def generate_code(self, parser_result):
        if parser_result.reflection_kind != self.reflection_kind:
            return ""

        if self.mute_token in parser_result.tokens:
            return ""

        class_name = parser_result.get_class_name()
        return (f"static_assert(IsObjectClassInherited); "
                f"// Class using ZT_REFLECT_CLASS should inherit public from Object class\n")
