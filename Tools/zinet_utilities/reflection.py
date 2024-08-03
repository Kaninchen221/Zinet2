import argparse
import os
import subprocess
import sys

args = f" -m zinet_reflector"
command = f"{sys.executable} " + args
process = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                         universal_newlines=True)
print(process.stdout)
print(process.stderr)

if process.returncode != 0:
    print(f"Reflector return code {process.returncode}")
    sys.exit(1)
