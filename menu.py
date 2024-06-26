from consolemenu import ConsoleMenu, SelectionMenu
from consolemenu.items import MenuItem, FunctionItem

import subprocess

from pathlib import Path

class Menu:
       
        __menuItems = []
        __menu = ConsoleMenu("Zinet Utilities", "The outputs from scripts are in Scripts/output folder")

        def run_script(self, script_file_name, arguments = ""):
                scriptFilePath = str(Path('.').absolute()) + "/Scripts/" + script_file_name + ' ' + arguments
                process = subprocess.run(scriptFilePath, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, universal_newlines=True)

                outputFileName = "output_" + script_file_name + ".temp";
                outputFilePath = str(Path('.')) + "/Scripts/outputs/" + outputFileName
                with open(outputFilePath, 'w') as file:
                        if process.stdout is not None: file.write(process.stdout)
                        if process.stderr is not None: file.write(process.stderr)
                        file.write("Return code: " + str(process.returncode))

                if process.returncode != 0:
                       if process.stdout is not None: raise Exception(str(process.stdout))
                       if process.stderr is not None: raise Exception(str(process.stderr))

        
        def main_menu(self):
                self.__menuItems = [
                        FunctionItem("Generate Project Files Address Sanitizer Off", self.run_script, ["generate_project.py", "--AddressSanitizer false"]),
                        FunctionItem("Generate Project Files Address Sanitizer On", self.run_script, ["generate_project.py", "--AddressSanitizer true"]),
                        FunctionItem("Generate Reflection", self.run_script, ["reflection.py"]),
                        FunctionItem("Conan Install Debug", self.run_script, ["conan_install.py", "--BuildType Debug"]),
                        FunctionItem("Conan Install Release", self.run_script, ["conan_install.py", "--BuildType Release"]),
                        FunctionItem("Build Project Debug", self.run_script, ["build.py", "--BuildType Debug"]),
                        FunctionItem("Build Project Release", self.run_script, ["build.py", "--BuildType Release"]),
                        FunctionItem("Compile Project Debug", self.run_script, ["compile.py", "--BuildType Debug"]),
                        FunctionItem("Compile Project Release", self.run_script, ["compile.py", "--BuildType Release"]),
                        FunctionItem("Run Automatic Tests", self.run_script, ["run_automatic_tests.py"]),
                        FunctionItem("Run All Tests", self.run_script, ["run_all_tests.py"]),
                        FunctionItem("Run All Tools Tests", self.run_script, ["run_tools_tests.py"]),
                        FunctionItem("Clear Build Folder", self.run_script, ["build_clear.bat"]),
                        FunctionItem("First Time", self.run_script, ["first_time.py"])
                ]

                for item in self.__menuItems:
                        self.__menu.append_item(item)

                self.__menu.show()

                #menu.join()


if __name__ == "__main__":
    menu = Menu()
    menu.run_script("first_time.py")
    menu.main_menu()