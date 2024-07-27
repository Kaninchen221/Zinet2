import argparse
import os
import subprocess
import sys

from zinet_utilities.paths import find_zinet_build_path, find_zinet_root_path, find_conan_profiles_folder, \
    get_build_folder_name

parser = argparse.ArgumentParser(description='CMake Build')

parser.add_argument('--BuildType', type=str, help='Debug or Release', default='Debug')

args = parser.parse_args()

print('BuildType:', args.BuildType)

if args.BuildType != "Debug" and args.BuildType != "Release":
    raise Exception("Invalid --BuildType")

if args.BuildType == "Debug":
    profile_file_name = "conan_profile_windows_vs_debug.txt"
else:
    profile_file_name = "conan_profile_windows_vs_release.txt"

profile_path = find_conan_profiles_folder() / profile_file_name
print(f"Profile path: {profile_path}")

zinet_root_path = find_zinet_root_path()
print(f"Zinet root path: {zinet_root_path}")

build_folder_path = find_zinet_build_path()
print(f"Zinet build folder path: {build_folder_path}")

subprocess.run("conan remove --locks", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
               universal_newlines=True)

arguments = (rf'install {zinet_root_path} '
             f'-pr {profile_path} '
             f'-pr:b {profile_path} '
             f'--build="missing" '
             f'-if {build_folder_path} '
             f'-of {zinet_root_path} ')
print(f"Arguments: {arguments}")

process = subprocess.run("conan " + arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                         universal_newlines=True)
print(process.stdout)
print(process.stderr)

if process.returncode != 0:
    print(f"Conan return code {process.returncode}")
    sys.exit(1)
