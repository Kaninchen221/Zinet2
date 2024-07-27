from pathlib import Path
import platform


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


def find_venv_folder():
    return find_zinet_root_path() / ".venv"


def find_venv_scripts_folder():
    if platform.system() == "Windows":
        scripts_folder = find_venv_folder() / "Scripts"
    elif platform.system() == "Linux":
        scripts_folder = find_venv_folder() / "bin"
    else:
        raise Exception("Not supported os")
    return scripts_folder


def find_venv_python_path():
    if platform.system() == "Windows":
        python_path = find_venv_scripts_folder() / "python.exe"
    elif platform.system() == "Linux":
        python_path = find_venv_scripts_folder() / "python3"
    else:
        raise Exception("Not supported os")
    return python_path
