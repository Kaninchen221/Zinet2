from pathlib import Path

from zinet_reflector.parser_result import *


class CodeInjector:
    def __init__(self):
        self.token_inject_code_start = "/*GENERATED_CODE_START*/"
        self.token_inject_code_end = "/*GENERATED_CODE_END*/"

    def inject_code(self, generated_code):
        for file_path, code in generated_code.items():
            if not Path(file_path).exists():
                raise Exception(f"File doesn't exist: {file_path}")

            with open(file_path, 'r+') as file:
                file_content = file.read()
                try:
                    inject_code_start_index = (file_content.index(self.token_inject_code_start)
                                               + len(self.token_inject_code_start))
                    inject_code_end_index = file_content.index(self.token_inject_code_end)
                except ValueError:
                    #print(f"Ignore {file_path} It hasn't injection tokens")
                    continue

            with open(file_path, 'w') as file:
                print(f"Inject code into: {file_path}")

                code = list(filter(lambda element: element is not None, code))
                generated_code_as_string = '\n' + '\n'.join(code)
                generated_code_as_string = CodeInjector.inject_indentations(generated_code_as_string, '\t\t')
                file_content = (file_content[:inject_code_start_index]
                                + generated_code_as_string
                                + "\n"
                                + file_content[inject_code_end_index:])
                file.write(file_content)

    @staticmethod
    def inject_indentations(string, indentation):
        return string.replace('\n', '\n' + indentation)
