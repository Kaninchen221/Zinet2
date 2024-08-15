from pathlib import Path

from zinet_reflector.parser import *
from zinet_reflector.reflector import Reflector
from zinet_utilities.paths import find_tools_folder


class TestReflector:

    def test_reflector(self):
        print('')
        project_root_folder_path = find_tools_folder() / "tests/zinet_reflector/test_files"
        folder_for_temp_main = project_root_folder_path / "main"
        reflector = Reflector()
        reflector.reflect(project_root_folder_path, folder_for_temp_main)
