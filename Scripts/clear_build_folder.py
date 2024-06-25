import argparse
import os
import shutil
import subprocess
import sys

from zinet_utilities.paths import find_zinet_root_path, find_zinet_build_path

build_folder_path = find_zinet_build_path()
print(f"Build folder path: {build_folder_path}")

shutil.rmtree(build_folder_path)
