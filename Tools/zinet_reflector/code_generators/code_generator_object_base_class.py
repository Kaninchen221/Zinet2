from zinet_reflector.code_generator import CodeGeneratorInstructionBase
from zinet_reflector.parser_result import ReflectionKind


class CodeGeneratorObjectBaseClass(CodeGeneratorInstructionBase):
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None

    def generate_code(self, parser_result):
        if parser_result.reflection_kind != self.reflection_kind:
            return ""

        result = ""
        if "NO_TEST_BASE_CLASS_OBJECT" not in parser_result.tokens:
            result += (f"static_assert(IsObjectClassInherited); "
                       f"// Class using ZT_REFLECT_CLASS should inherit public from Object class\n")

        class_name = parser_result.get_class_name()

        if "NO_REGISTER_CLASS" not in parser_result.tokens:
            result += f"const inline static bool RegisterClassResult = RegisterClass<{class_name}>();\n"

        if "NO_CREATE_COPY" not in parser_result.tokens:
            result += (f"std::unique_ptr<ObjectBase> createCopy() const override {{ "
                       f"std::unique_ptr<ObjectBase> result = createCopyInternal<{class_name}>(); "
                       f"*result = *this; "
                       f"return result; }}\n")

        return result
