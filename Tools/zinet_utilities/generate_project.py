import argparse
import subprocess
import sys

from zinet_utilities.paths import find_zinet_root_path, find_zinet_build_path

parser = argparse.ArgumentParser(description='Generate project files')
parser.add_argument('--AddressSanitizer', type=str, help='true or false', default='false')
args = parser.parse_args()

if args.AddressSanitizer != "true" and args.AddressSanitizer != "false":
    raise Exception("Incorrect AddressSanitizer arg")

zinet_root_path = find_zinet_root_path()
print(f"Zinet root path: {zinet_root_path}")

build_folder_path = find_zinet_build_path()
print(f"Build folder path: {build_folder_path}")

args = f"-m zinet_generator --AddressSanitizer {args.AddressSanitizer}"
final_command = f"{sys.executable} " + args
print(f"Final command: {final_command}")

process = subprocess.run(final_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                         universal_newlines=True)
print(process.stdout)
print(process.stderr)

if process.returncode != 0:
    print(f"Generate project files return code {process.returncode}")
    sys.exit(1)
