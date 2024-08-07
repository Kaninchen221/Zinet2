from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorConstructors(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None
        self.mute_token = "NO_CONSTRUCTORS"

    def generate_code(self, parser_result):
        if parser_result.reflection_kind != self.reflection_kind:
            return ""

        if self.mute_token in parser_result.tokens:
            return ""

        class_name = parser_result.get_class_name()
        return (f"{class_name}() = default;\n"
                f"{class_name}(const {class_name}& other) = default;\n"
                f"{class_name}({class_name}&& other) = default;\n\n"
                f"~{class_name}() noexcept = default;\n")
