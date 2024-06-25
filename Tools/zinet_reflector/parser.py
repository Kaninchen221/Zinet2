import os

import clang.cindex
from zinet_reflector.parser_result import *


class Parser:
    def __init__(self):
        self.root_cursor = {}
        self._ignored_cursor_kinds = [CursorKind.MACRO_DEFINITION, CursorKind.INCLUSION_DIRECTIVE]
        self._include_paths = []
        self._include_folder_name = "include"

    def parse(self, raw_path):
        print(f"Parse file: {raw_path}")
        parser = clang.cindex.Index.create()
        options = self.get_options()
        args = self._get_args()

        cindex_parser_result = parser.parse(raw_path, args, unsaved_files=None, options=options)

        #Handling parse error
        #if cindex_parser_result.diagnostics:
            #print("Diagnostics in parse_result is not empty")
            #assert False

        self.root_cursor = cindex_parser_result.cursor
        if not self.root_cursor.get_children():
            print("Cursor.get_children returned empty")
            assert False

        parser_result = ParserResult()
        parser_result.cursor = self.root_cursor
        self._parse_internal(parser_result)
        #print_parser_result(parser_result)
        return parser_result

    def _parse_internal(self, parent_parser_result):
        for child_cursor in parent_parser_result.cursor.get_children():
            if not child_cursor.displayname:
                continue

            if child_cursor.kind in self._ignored_cursor_kinds:
                #print(child_cursor.displayname)
                continue

            new_parser_result = ParserResult()
            new_parser_result.cursor = child_cursor
            self._parse_internal(new_parser_result)
            parent_parser_result.children.append(new_parser_result)

    @staticmethod
    def get_options():
        return (clang.cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD +
                clang.cindex.TranslationUnit.PARSE_INCOMPLETE +
                clang.cindex.TranslationUnit.PARSE_PRECOMPILED_PREAMBLE +
                clang.cindex.TranslationUnit.PARSE_CACHE_COMPLETION_RESULTS +
                clang.cindex.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES +
                clang.cindex.TranslationUnit.PARSE_INCLUDE_BRIEF_COMMENTS_IN_CODE_COMPLETION)

    @staticmethod
    def _get_args():
        args = []

        args.append("-nostdinc++")
        # -march=native -flto -fomit-frame-pointer -O3 -DNDEBUG
        #args.append("-march=native")
        #args.append("-flto")
        #args.append("-fomit-frame-pointer")
        #args.append("-O3")
        #args.append("-DNDEBUG")

        return args

    def _get_include_path_args(self, project_root_folder):
        result = []
        for root, dir_names, files in os.walk(project_root_folder):
            for dir_name in dir_names:
                dir_absolute_path = root + '\\' + dir_name
                if dir_absolute_path not in self._include_paths:
                    if dir_name == self._include_folder_name:
                        # print(f"Found include dir: {dir_absolute_path}")
                        self._include_paths.append(dir_absolute_path)
                        result.append(f"-I{dir_absolute_path}")
        return result
