import shutil

from zinet_utilities.paths import find_zinet_root_path, find_zinet_build_path

build_folder_path = find_zinet_build_path()
print(f"Build folder path: {build_folder_path}")

if build_folder_path.exists():
    shutil.rmtree(build_folder_path)
    print("Build folder cleared")
else:
    raise Exception("Build folder doesn't exist")
