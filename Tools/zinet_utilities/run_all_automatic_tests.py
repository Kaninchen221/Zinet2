import os
import subprocess
import sys

from zinet_utilities.paths import find_zinet_bin_path

from zinet_utilities.platform_info import get_system, SystemInfo

bin_folder_path = find_zinet_bin_path()
print(f"Bin folder path: {bin_folder_path}")

for root, subdir, files in os.walk(bin_folder_path):
    for file in files:
        file_path = os.path.join(root, file)
        if get_system() == SystemInfo.Windows and (not file.endswith(".exe") or file.find("Automatic") == -1):
            continue

        if get_system() == SystemInfo.Linux and file.find("Automatic") == -1:
            continue

        if get_system() == SystemInfo.Other:
            raise Exception("Not supported OS Platform")

        command = str(file_path)
        print("Run: " + command)
        process = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                                 universal_newlines=True)
        print(process.stdout)
        print(process.stderr)

        if process.returncode != 0:
            print(f"Process {file} return code {process.returncode}")
            sys.exit(1)
