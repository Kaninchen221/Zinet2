import concurrent.futures
import os
import timeit
from pathlib import Path

import clang
import winsound

from zinet_reflector.assignor import Assignor
from zinet_reflector.code_generator import CodeGenerator, print_generated_code, CodeGeneratorInstructionBase
from zinet_reflector.code_generators.code_generator_class_info import CodeGeneratorClassInfo
from zinet_reflector.code_injector import CodeInjector
from zinet_reflector.entry_point_main import EntryPointMain
from zinet_reflector.parser import Parser
from zinet_reflector.parser_result import print_parser_result
from zinet_reflector.tokens_finder import TokensFinder
from zinet_reflector.parser_result import *

from zinet_reflector.code_generators.code_generator_constructors import CodeGeneratorConstructors
from zinet_reflector.code_generators.code_generator_getter_setter import CodeGeneratorGetterSetter
from zinet_reflector.code_generators.code_generator_operators import CodeGeneratorOperators
from zinet_utilities.paths import find_tools_folder


class Reflector:

    def __init__(self):
        self.main_name = "main.cpp"
        self.found_file_paths = []

    def reflect(self, project_root_folder_path, folder_for_temp_main, exceptions_paths=None):
        if exceptions_paths is None:
            exceptions_paths = []

        print(f"Project root folder: {project_root_folder_path}")

        entry_point_main = EntryPointMain()
        entry_point_main.create_main(project_root_folder_path,
                                     folder_for_temp_main,
                                     project_root_folder_path,
                                     exceptions_paths)

        self._reflect_files_internal([entry_point_main.main_file_path])

    @staticmethod
    def load_libclang_dll():
        library_path = find_tools_folder() / "libclang.dll"
        library_raw_path = str(library_path)
        print(f"Set libclang.dll path: {library_raw_path}")
        assert library_path.is_file()
        clang.cindex.Config.set_library_file(library_raw_path)

    def _find_file_for_reflection(self, path):
        for root, dirs, files in os.walk(path):
            for file in files:
                if file == self.main_name:
                    return root + '/' + file

    def reflect_multiple(self, project_root_folder):
        print(f"Find multiple {self.main_name} under path: {project_root_folder}")
        self.found_file_paths = self._find_files_for_reflection(project_root_folder)
        print(f"Found at: {' '.join(self.found_file_paths)}")
        self._reflect_files_internal(self.found_file_paths)

    def _find_files_for_reflection(self, path):
        found_file_paths = []
        for root, dirs, files in os.walk(path):
            for file in files:
                if file == self.main_name:
                    found_file_paths.append(root + '/' + file)
        return found_file_paths

    def _reflect_files_internal(self, files_paths):
        for file_path in files_paths:
            self._reflect_file(file_path)

    @staticmethod
    def _reflect_file(file_path):
        print(f"Start reflection for file: {file_path}")
        reflection_start_time = timeit.default_timer()

        raw_file_path = str(file_path)
        parser = Parser()

        parsing_start_time = timeit.default_timer()
        parse_result = parser.parse(raw_file_path)
        parsing_end_time = timeit.default_timer()
        parser_elapsed_seconds = parsing_end_time - parsing_start_time
        print(f"Parsing took {parser_elapsed_seconds} seconds")

        assignor_start_time = timeit.default_timer()
        assignor = Assignor()
        assignor.assign(parse_result)
        #print_parser_result(parse_result)
        assignor_end_time = timeit.default_timer()
        assignor_elapsed_seconds = assignor_end_time - assignor_start_time
        print(f"Assignor took {assignor_elapsed_seconds} seconds")

        find_tokens_start_time = timeit.default_timer()
        tokens_finder = TokensFinder()
        tokens_finder.find_tokens(parse_result)
        find_tokens_end_time = timeit.default_timer()
        find_tokens_elapsed_seconds = find_tokens_end_time - find_tokens_start_time
        print(f"Findings tokens took {find_tokens_elapsed_seconds} seconds")

        #print_parser_result(parse_result)

        code_generator = CodeGenerator()
        subclasses = CodeGeneratorInstructionBase.__subclasses__()
        for subclass in subclasses:
            code_generator.instructions.append(subclass())

        generate_code_start_time = timeit.default_timer()
        generated_code = code_generator.generate_code(parse_result)
        generate_code_end_time = timeit.default_timer()
        generate_code_elapsed_seconds = generate_code_end_time - generate_code_start_time
        print(f"Generate code took {generate_code_elapsed_seconds} seconds")
        # print_generated_code(generated_code)

        inject_code_start_time = timeit.default_timer()
        code_injector = CodeInjector()
        code_injector.inject_code(generated_code)
        inject_code_end_time = timeit.default_timer()
        inject_code_elapsed_seconds = inject_code_end_time - inject_code_start_time
        print(f"Inject code took {format(inject_code_elapsed_seconds, '.18f')} seconds")

        reflection_end_time = timeit.default_timer()
        reflection_elapsed_seconds = reflection_end_time - reflection_start_time
        print(f"Reflection took {reflection_elapsed_seconds} seconds")

        print(f"End reflection for file: {file_path}")
