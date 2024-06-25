import os
import pathlib


class EntryPointMain:

    def __init__(self):
        self._headers_postfix = '.hpp'
        self.main_file_path = ""

    def create_main(self, project_root_folder_path, folder_for_temp_main, include_paths, exceptions_paths=None):
        if exceptions_paths is None:
            exceptions_paths = []

        #print(f"Find include paths under folder: {project_root_folder_path}")
        print(f"Create main.cpp in folder: {folder_for_temp_main}")
        print(f"Find headers under path: {include_paths}")

        headers = self._find_headers(include_paths, exceptions_paths)
        print(f"Found headers count: {len(headers)}")

        headers_as_string = self._headers_to_string(headers)
        #print(f"Headers as string: \n{headers_as_string}")

        main_file_content = self._create_main_file_string(headers_as_string)
        #print(f"Generated main file content: \n{main_file_content}")

        self._create_main_file(main_file_content, folder_for_temp_main)

    def _find_headers(self, folder_path, exceptions_paths):
        found_file_paths = []
        for root, dirs, files in os.walk(folder_path):
            skip = False
            for exception_path in exceptions_paths:
                if pathlib.Path(root).parent.is_relative_to(exception_path):
                    skip = True

            if skip:
                continue

            for file in files:
                if file.endswith(self._headers_postfix):
                    found_file_path = pathlib.Path(root + '/' + file)
                    if "Config" in file:
                        found_file_paths.insert(0, found_file_path)
                    else:
                        found_file_paths.append(found_file_path)

        return found_file_paths

    @classmethod
    def _headers_to_string(cls, headers):
        string = ""
        for header in headers:
            string = string + f"#include \"{str(header)}\"\n"
        return string

    @classmethod
    def _create_main_file_string(cls, headers_as_string):
        return f"{headers_as_string} \nint main() {{ return 0; }}"

    def _create_main_file(self, main_file_content, folder_for_temp_main):
        self.main_file_path = folder_for_temp_main / "main.temp.cpp"
        with (open(self.main_file_path, 'w') as file):
            file.write(main_file_content)
