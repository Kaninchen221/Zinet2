from enum import Enum
from clang.cindex import CursorKind


class ReflectionKind(Enum):
    Namespace = "ZT_REFLECT_NAMESPACE"
    Class = "ZT_REFLECT_CLASS"
    Struct = "ZT_REFLECT_STRUCT"
    Method = "ZT_REFLECT_METHOD"
    Member = "ZT_REFLECT_MEMBER"


class ParserResult:
    def __init__(self):
        self.cursor = None
        self.reflection_kind = None
        self.reflection_cursor = None
        self.children = []
        self.tokens = []

    def get_class_name(self):
        return self.cursor.displayname

    def get_member_class_type_name(self):
        return self.cursor.lexical_parent.spelling

    def get_member_name(self):
        return self.cursor.spelling

    def get_member_type(self):
        member_name = self.get_member_name()
        return f"decltype({member_name})"

    def get_member_type_name(self):
        name = self.cursor.type.spelling
        return name

    def get_member_getter_name(self):
        member_name = self.get_member_name()
        getter_name = member_name[0].upper() + member_name[1:]
        return f"get{getter_name}"

    def get_member_setter_name(self):
        member_name = self.get_member_name()
        setter_name = member_name[0].upper() + member_name[1:]
        return f"set{setter_name}"

    def get_cursor_file_path(self):
        return self.cursor.location.file.name


def print_parser_result(parser_result):
    _print_parser_result_internal(parser_result, "")


def _print_parser_result_internal(parser_result, spacing):
    display_name = parser_result.cursor.displayname

    if parser_result.cursor.location.file is None:
        kind = "File Root Cursor"
    else:
        kind = parser_result.cursor.kind
    line = parser_result.cursor.location.line

    reflect_macro_message = ""
    if parser_result.reflection_cursor is None:
        reflect_macro = "None"
        if parser_result.cursor.location.line != 0:
            if parser_result.cursor.kind != CursorKind.MACRO_INSTANTIATION:
                reflect_macro_message = f"assigned reflection: {reflect_macro}"
    else:
        reflect_macro = parser_result.reflection_kind
        reflect_macro_line = parser_result.reflection_cursor.location.line
        reflect_macro_message = f"assigned reflection: {reflect_macro} from line: {reflect_macro_line}"

    tokens_message = ""
    if parser_result.cursor.location.line != 0:
        if parser_result.cursor.kind == CursorKind.MACRO_INSTANTIATION:
            pass
        elif parser_result.reflection_cursor is not None:
            tokens_message = f"Tokens: {parser_result.tokens}"

    print(f"{spacing}{display_name} {kind} at line: {line} {reflect_macro_message} {tokens_message}")
    spacing += "    "
    for child in parser_result.children:
        _print_parser_result_internal(child, spacing)
    spacing = spacing[:-4]
