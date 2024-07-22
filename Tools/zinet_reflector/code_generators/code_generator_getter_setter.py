from clang.cindex import CursorKind

from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorGetterSetter(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Member
        self.token = "ReadWrite"
        self.token_only_getter = "ReadOnly"

    def generate_code(self, parser_result):
        if parser_result.reflection_kind == self.reflection_kind:
            if not hasattr(parser_result.cursor, "semantic_parent"):
                return ""

            # Ignore nested structs
            if parser_result.cursor.semantic_parent.kind == CursorKind.STRUCT_DECL:
                return ""

            member_name = parser_result.get_member_name()
            member_type_name = "decltype(" + parser_result.get_member_name() + ")"
            getter_name = parser_result.get_member_getter_name()

            if self.token in parser_result.tokens:
                setter_name = parser_result.get_member_setter_name()

                return (f"const {member_type_name}& {getter_name}() const {{ return {member_name}; }}\n"
                        f"void {setter_name}(const {member_type_name}& newValue) {{ {member_name} = newValue; }}")

            elif self.token_only_getter in parser_result.tokens:
                return f"const {member_type_name}& {getter_name}() const {{ return {member_name}; }}"
