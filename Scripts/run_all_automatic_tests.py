import os
import subprocess
import sys

from zinet_utilities.paths import find_zinet_bin_path

bin_folder_path = find_zinet_bin_path()
print(f"Bin folder path: {bin_folder_path}")
os.chdir(bin_folder_path)

for file in os.listdir():
    if file.endswith(".exe") and file.find("Automatic") != -1:
        process = subprocess.run(file, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                                 universal_newlines=True)
        print(process.stdout)
        print(process.stderr)

        if process.returncode != 0:
            print(f"Process {file} return code {process.returncode}")
            sys.exit(1)
