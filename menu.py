import argparse
import os
import subprocess
from pathlib import Path

from Tools.zinet_utilities.paths import find_tools_folder, find_venv_python_path


def cls():
    os.system('cls' if os.name == 'nt' else 'clear')


def print_centered(text):
    print('{:^60}'.format(f"{text}"))


class MenuOption:
    last_index = 0

    def __init__(self):
        self.name = ""
        self.pretty_name = ""
        self.run_script_function = None
        self.args = []
        self.index = 0

    @staticmethod
    def create(name, pretty_name, run_script_function, args):
        menu_option = MenuOption()
        menu_option.name = name
        menu_option.pretty_name = pretty_name
        menu_option.run_script_function = run_script_function
        menu_option.args = args
        menu_option.index = MenuOption.last_index
        MenuOption.last_index += 1
        return menu_option


def exit_program():
    exit(0)


class Menu:
    def __init__(self):
        self.menu_options = []
        self.menu_title = "Zinet Utilities"
        self.description = "The outputs from scripts are in Scripts/output folder"
        self.add_options()

    @staticmethod
    def run_script(script_file_name, arguments):
        script_file_path = str(find_tools_folder() / "zinet_utilities" / script_file_name)
        if not Path(script_file_path).exists():
            raise Exception(f"Script with path: {script_file_path} doesn't exist")

        script_file_final_path = str(find_venv_python_path()) + " " + script_file_path + " " + ' '.join(arguments)
        process = subprocess.run(script_file_final_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                                 universal_newlines=True)

        output_file_name = "output_" + script_file_name + ".temp"
        output_file_folder = find_tools_folder() / "outputs"
        if not output_file_folder.exists():
            raise Exception(f"Scripts outputs folder: {output_file_folder} doesn't exist")

        output_file_path = str(output_file_folder) + '/' + output_file_name

        with open(output_file_path, 'w') as file:
            if process.stdout:
                file.write(str(process.stdout))
            if process.stderr:
                file.write(str(process.stderr))
            file.write("Return code: " + str(process.returncode))

        if process.stdout:
            print(process.stdout)

        if process.stderr:
            print(process.stderr)

        if process.returncode != 0:
            raise Exception(str(process.stdout) + ' : ' + str(process.stderr))

    def print_options(self):
        menu_text = ""
        option_index = 0
        for menu_option in self.menu_options:
            menu_text += f"    {option_index}: {menu_option.pretty_name}\n"
            option_index += 1
        print(menu_text)

    def execute_option_index(self, option_index):
        menu_option = self.menu_options[option_index]
        if menu_option.args:
            menu_option.run_script_function(menu_option.args[0], menu_option.args[1:])
        else:
            menu_option.run_script_function()

    def execute_option_index_safe(self, option_index):
        try:
            self.execute_option_index(option_index)
        except Exception as exception:
            print(f"\n{exception}\n")

    def execute_option_name(self, option_name):
        menu_option = [option for option in self.menu_options if option.name == option_name][0]
        self.execute_option_index(menu_option.index)

    def execute_option_name_safe(self, option_name):
        try:
            self.execute_option_name(option_name)
        except IndexError as exception:
            print(f"\n{exception}\nIndex out of range could be an invalid name\n")
        except Exception as exception:
            print(f"\n{exception}\n")

    def add_options(self):
        self.menu_options = [
            MenuOption.create("generate_project_address_sanitizer_off",
                              "Generate Project Files Address Sanitizer Off", self.run_script,
                              ["generate_project.py", "--AddressSanitizer false"]),
            MenuOption.create("generate_project_address_sanitizer_on",
                              "Generate Project Files Address Sanitizer On", self.run_script,
                              ["generate_project.py", "--AddressSanitizer true"]),
            MenuOption.create("generate_reflection", "Generate Reflection", self.run_script,
                              ["reflection.py"]),

            MenuOption.create("conan_install_windows_msvc_debug", "Conan Install Windows MSVC Debug", self.run_script,
                              ["conan_install.py", "--BuildType Debug --Compiler MSVC --Platform Windows"]),
            MenuOption.create("conan_install_windows_msvc_release", "Conan Install Windows MSVC Release", self.run_script,
                              ["conan_install.py", "--BuildType Release --Compiler MSVC --Platform Windows"]),

            MenuOption.create("conan_install_linux_gcc_debug", "Conan Install Linux GCC Debug", self.run_script,
                              ["conan_install.py", "--BuildType Debug --Compiler GCC --Platform Linux"]),
            MenuOption.create("conan_install_linux_gcc_release", "Conan Install Linux GCC Release", self.run_script,
                              ["conan_install.py", "--BuildType Release --Compiler GCC --Platform Linux"]),

            MenuOption.create("build_project_debug_visual_studio", "Build Project Debug Visual Studio 17 2022", self.run_script,
                              ["build.py", '--BuildType Debug --CMakeGenerator "Visual Studio 17 2022"']),
            MenuOption.create("build_project_release_visual_studio", "Build Project Release Visual Studio 17 2022", self.run_script,
                              ["build.py", '--BuildType Release --CMakeGenerator "Visual Studio 17 2022"']),

            MenuOption.create("build_project_debug_ninja", "Build Project Debug Ninja", self.run_script,
                              ["build.py", '--BuildType Debug --CMakeGenerator "Ninja"']),
            MenuOption.create("build_project_release_ninja", "Build Project Release Ninja", self.run_script,
                              ["build.py", '--BuildType Release --CMakeGenerator "Ninja"']),

            MenuOption.create("compile_project_debug", "Compile Project Debug", self.run_script,
                              ["compile.py", "--BuildType Debug"]),
            MenuOption.create("compile_project_release", "Compile Project Release", self.run_script,
                              ["compile.py", "--BuildType Release"]),

            MenuOption.create("run_all_automatic_tests", "Run All Automatic Tests", self.run_script,
                              ["run_all_automatic_tests.py"]),
            MenuOption.create("run_all_tools_tests", "Run All Tools Tests", self.run_script,
                              ["run_tools_tests.py"]),
            MenuOption.create("clear_build_folder", "Clear Build Folder", self.run_script,
                              ["clear_build_folder.py"]),
            MenuOption.create("exit", "Exit", exit_program, None)
        ]

    def main_menu(self):
        cls()
        while True:

            print()
            print_centered(self.menu_title)
            print_centered(self.description)
            print()

            self.print_options()

            option_index = input("Select option: ")
            if option_index.isdigit():
                option_index = int(option_index)
                cls()
                self.execute_option_index_safe(option_index)
            else:
                cls()

                
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Menu.py')
    parser.add_argument('--OptionIndex', type=int, help='Option index that should be executed', default='-1')
    parser.add_argument('--OptionName', type=str, help='Option name that should be executed', default="invalid_name")
    args = parser.parse_args()

    menu = Menu()
    if args.OptionIndex != -1:
        menu.execute_option_index(args.OptionIndex)
    elif args.OptionName != "invalid_name":
        menu.execute_option_name(args.OptionName)
    else:
        menu.main_menu()


