import argparse
import os
import subprocess
import sys

from zinet_utilities.paths import find_zinet_root_path, find_zinet_build_path

parser = argparse.ArgumentParser(description='CMake Build')
parser.add_argument('--BuildType', type=str, help='Debug or Release', default='Debug')
parser.add_argument('--CMakeGenerator', type=str, help='CMake Generator', default='Visual Studio 17 2022')
args = parser.parse_args()

print('BuildType:', args.BuildType)

if args.BuildType != "Debug" and args.BuildType != "Release":
    raise Exception("Invalid --BuildType")

zinet_root_path = find_zinet_root_path()
print(f"Zinet root path: {zinet_root_path}")

build_folder_path = find_zinet_build_path()
print(f"Build folder path: {build_folder_path}")

arguments = (f'-S {zinet_root_path} -B {build_folder_path} -G "{args.CMakeGenerator}" '
             f'-D CMAKE_BUILD_TYPE={args.BuildType} '
             f'-D BUILD_SHARED_LIBS:BOOL=FALSE '
             f'-D SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -D SAMPLES_BUILD=ON '
             f'-D TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -D TESTS_BUILD=ON')
print(f"Arguments: {arguments}")

process = subprocess.run("cmake " + arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                         universal_newlines=True)
print(process.stdout)
print(process.stderr)
    
if process.returncode != 0:
    print(f"CMake return code {process.returncode}")
    sys.exit(1)
