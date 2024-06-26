import os
import subprocess
import sys

from zinet_utilities.paths import find_tools_folder

tools_folder_path = find_tools_folder()
print(f"Tools folder path: {tools_folder_path}")
os.chdir(tools_folder_path)

process = subprocess.run("pytest", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, universal_newlines=True)
print(process.stdout)
print(process.stderr)
    
if process.returncode != 0:
    print(f"Pytest return code {process.returncode}")
    sys.exit(1)
