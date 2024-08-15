import types

from zinet_reflector.parser_result import *


class Assignor:
    def __init__(self):
        self._not_assigned_reflection_parse_results = []
        self._assigned_reflections = 0
        self._reflection_macro_must_contains = "ZT_REFLECT_"
        self._ignor_cursor_kinds = [CursorKind.MACRO_INSTANTIATION, CursorKind.CXX_BASE_SPECIFIER, CursorKind.TYPE_REF]

    def assign(self, parser_result):
        self._find_reflection_parse_results(parser_result)
        self._assign_internal(parser_result)
        print(f"Not assigned reflection parse results count: {len(self._not_assigned_reflection_parse_results)}")
        print(f"Assigned reflections: {self._assigned_reflections}")

    def _find_reflection_parse_results(self, parser_result):
        for child_parser_result in parser_result.children:
            if child_parser_result.cursor.kind != CursorKind.MACRO_INSTANTIATION:
                #print(child_parser_result.cursor.displayname)
                continue

            if self._reflection_macro_must_contains not in child_parser_result.cursor.displayname:
                #print(child_parser_result.cursor.displayname)
                continue

            self._not_assigned_reflection_parse_results.append(child_parser_result)
            #print(child_parser_result.cursor.displayname)
            self._find_reflection_parse_results(child_parser_result)

    def _assign_internal(self, parser_result):
        for child_parser_result in parser_result.children:

            child_line = child_parser_result.cursor.location.line
            child_file = child_parser_result.cursor.location.file
            for _not_assigned_reflection_parse_result in self._not_assigned_reflection_parse_results:

                child_cursor_kind = child_parser_result.cursor.kind
                if child_cursor_kind in self._ignor_cursor_kinds:
                    continue

                if (child_line == _not_assigned_reflection_parse_result.cursor.location.line + 1 and
                        child_file.name == _not_assigned_reflection_parse_result.cursor.location.file.name):
                    try:
                        child_parser_result.reflection_cursor = _not_assigned_reflection_parse_result.cursor
                        child_parser_result.reflection_kind = ReflectionKind(
                            child_parser_result.reflection_cursor.displayname)
                        self._assigned_reflections += 1
                        self._not_assigned_reflection_parse_results.remove(_not_assigned_reflection_parse_result)
                        print(f"Found: {child_parser_result.reflection_cursor.displayname} -> {child_parser_result.cursor.displayname} in file {child_file.name} kind {child_parser_result.cursor.kind}")
                    except KeyError:
                        print(f"KeyError for: {_not_assigned_reflection_parse_result.cursor.displayname} in file {child_file.name} at line {_not_assigned_reflection_parse_result.cursor.location.line}")
                        pass
                    except ValueError:
                        print(f"ValueError for: {_not_assigned_reflection_parse_result.cursor.displayname} in file {child_file.name} at line {_not_assigned_reflection_parse_result.cursor.location.line}")
                        pass

            self._assign_internal(child_parser_result)
