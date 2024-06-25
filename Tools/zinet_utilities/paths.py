from pathlib import Path


def is_zinet_root_path(path):
    if not (path / "recipe_root.py").exists():
        return False
    return True


def find_zinet_root_path():
    root_path = Path('.').absolute()
    if is_zinet_root_path(root_path):
        return root_path

    for parent in root_path.parents:
        if is_zinet_root_path(parent):
            return parent
    return None


def get_build_folder_name():
    return "build"


def find_zinet_build_path():
    return find_zinet_root_path() / get_build_folder_name()


def find_zinet_bin_path():
    return find_zinet_build_path() / "bin"


def find_tools_folder():
    return find_zinet_root_path() / "Tools"


def find_scripts_folder():
    return find_zinet_root_path() / "Scripts"


def find_conan_profiles_folder():
    return find_scripts_folder() / "conan_profiles"
