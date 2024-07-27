import argparse
import os
import subprocess
import sys

from zinet_utilities.paths import find_zinet_root_path, find_zinet_build_path

parser = argparse.ArgumentParser(description='Compile All')
parser.add_argument('--BuildType', type=str, help='Debug or Release', default='Debug')
args = parser.parse_args()

print('BuildType:', args.BuildType)

if args.BuildType != "Debug" and args.BuildType != "Release":
    raise Exception("Invalid --BuildType")

build_folder_path = find_zinet_build_path()
print(f"Build folder path: {build_folder_path}")

arguments = f'--build {build_folder_path} --config {args.BuildType}'
print(f"Cmake arguments: '{arguments}'")

process = subprocess.run("cmake " + arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                         universal_newlines=False)

print(f"process.stdout: {process.stdout}")
print(f"process.stderr: {process.stderr}")

if process.returncode != 0:
    print(f"CMake return code {process.returncode}")
    sys.exit(1)
