from zinet_reflector.parser_result import ReflectionKind
from clang.cindex import CursorKind


class ClassInfoParentsChildren:
    def __init__(self):
        super().__init__()
        self.reflection_kind = ReflectionKind.Class
        self.token = None
        self.parents = []
        self.class_name = ""

    def generate_code(self, parser_result):
        if parser_result.reflection_kind != self.reflection_kind:
            return ""

        children_cursors = parser_result.cursor.get_children()
        self.class_name = parser_result.cursor.spelling
        for child_cursor in children_cursors:
            if child_cursor.kind == CursorKind.CXX_BASE_SPECIFIER:
                parent_class_name = child_cursor.spelling
                self.parents.append(parent_class_name)
        return ""

    def generate_code_post(self):
        result = ""
        result += self.generate_parents_class_Info()
        return result

    def generate_parents_class_Info(self):
        if self.parents:
            initializer_list = "{"
            for parent in self.parents:
                initializer_list += f"{parent}::ClassInfo{{}},"
            initializer_list = initializer_list[0:-1]
            initializer_list += '}'
            return f"\n\tconstexpr static auto GetParentsClassInfo() {{ return std::vector{initializer_list}; }}"
        return ""

    def __str__(self):
        if self.parents:
            return f"Class: {self.class_name} has parents: {self.parents}"
        else:
            return f"Class: {self.class_name} hasn't any parents classes"

    parentsPerClass = {}

