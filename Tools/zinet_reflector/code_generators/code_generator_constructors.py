from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorConstructors(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None
        self.mute_constructors = "NO_CONSTRUCTORS"
        self.mute_destructor = "NO_DESTRUCTOR"

    def generate_code(self, parser_result):
        if parser_result.reflection_kind != self.reflection_kind:
            return ""

        result = ""
        if self.mute_constructors not in parser_result.tokens:
            class_name = parser_result.get_class_name()
            result = (f"{class_name}() = default;\n"
                      f"{class_name}(const {class_name}& other) = default;\n"
                      f"{class_name}({class_name}&& other) = default;\n")

        if self.mute_destructor not in parser_result.tokens:
            class_name = parser_result.get_class_name()
            result += f"~{class_name}() noexcept = default;\n"

        return result
