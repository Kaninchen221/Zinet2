from pathlib import Path

from zinet_reflector.entry_point_main import EntryPointMain
from zinet_utilities.paths import find_tools_folder, find_zinet_root_path


class TestEntryPointMain:

    def test_entry_point_main(self):
        project_root_folder_path = find_zinet_root_path()
        entry_point_main_folder_path = find_tools_folder() / r"tests/zinet_reflector/test_files/main"
        includes_paths = find_tools_folder() / r"tests/zinet_reflector/test_files/include"
        entry_point_main = EntryPointMain()
        entry_point_main.create_main(project_root_folder_path, entry_point_main_folder_path, includes_paths)

        assert entry_point_main.main_file_path.is_file()
