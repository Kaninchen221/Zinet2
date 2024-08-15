from zinet_reflector.reflector import Reflector
from zinet_utilities.paths import find_zinet_root_path


if __name__ == '__main__':
    try:
        print("Zinet Reflector")
        reflector = Reflector()
        Reflector.load_libclang_dll()
        project_root_folder_path = find_zinet_root_path()
        if project_root_folder_path is None:
            raise Exception("Can't find zinet root folder path")

        folder_for_temp_main = project_root_folder_path / "Tools"
        exceptions_paths = [folder_for_temp_main]
        reflector.reflect(project_root_folder_path, folder_for_temp_main, exceptions_paths)
    except Exception as exception:
        print(exception.args)
        exit(1)
