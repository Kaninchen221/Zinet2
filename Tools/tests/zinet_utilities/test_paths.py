from zinet_utilities import paths

from zinet_utilities.platform_info import get_system, SystemInfo


class TestUtilities:
    def test_find_zinet_root_path(self):
        zinet_root_path = paths.find_zinet_root_path()
        assert zinet_root_path
        assert zinet_root_path.exists()

        assert zinet_root_path.parent.name != "tests"
        assert zinet_root_path.parent.parent.name != "Tools"

    def test_get_build_folder_name(self):
        actual = paths.get_build_folder_name()
        expected = "build"
        assert actual == expected

    def test_find_zinet_build_path(self):
        actual = paths.find_zinet_build_path()
        expected = paths.find_zinet_root_path() / "build"
        assert actual == expected

    def test_find_zinet_bin_path(self):
        actual = paths.find_zinet_bin_path()
        expected = paths.find_zinet_build_path() / "bin"
        assert actual == expected

    def test_find_tools_folder(self):
        actual = paths.find_tools_folder()
        expected = paths.find_zinet_root_path() / "Tools"
        assert actual == expected

    def test_find_conan_profiles_folder(self):
        actual = paths.find_conan_profiles_folder()
        expected = paths.find_tools_folder() / "conan_profiles"
        assert actual == expected

    def test_find_status_icons_folder(self):
        actual = paths.find_status_icons_folder()
        expected = paths.find_tools_folder() / "status_icons"
        assert actual == expected
        assert expected.exists()

    def test_find_venv_folder(self):
        actual = paths.find_venv_folder()
        expected = paths.find_zinet_root_path() / ".venv"
        assert actual == expected

    def test_find_venv_scripts_folder(self):
        venv_scripts_folder = paths.find_venv_scripts_folder()

        if get_system() == SystemInfo.Windows:
            expected = paths.find_venv_folder() / "Scripts"
        elif get_system() == SystemInfo.Linux:
            expected = paths.find_venv_folder() / "bin"
        else:
            raise Exception("Not supported os")

        assert venv_scripts_folder.exists()
        assert venv_scripts_folder == expected

    def test_find_venv_activate_path(self):
        venv_activate_path = paths.find_venv_activate_folder()

        if get_system() == SystemInfo.Windows:
            expected = paths.find_venv_scripts_folder() / "activate.bat"
        elif get_system() == SystemInfo.Linux:
            expected = paths.find_venv_scripts_folder() / "activate"
        else:
            raise Exception("Not supported os")

        assert venv_activate_path.exists()
        assert venv_activate_path == expected

    def test_find_venv_python_path(self):
        venv_python_path = paths.find_venv_python_path()

        if get_system() == SystemInfo.Windows:
            expected = paths.find_venv_folder() / "Scripts/python.exe"
        elif get_system() == SystemInfo.Linux:
            expected = paths.find_venv_folder() / "bin/python3"
        else:
            raise Exception("Not supported os")

        assert venv_python_path.exists()
        assert venv_python_path == expected

    def test_find_venv_pip_path(self):
        venv_pip_path = paths.find_venv_pip_path()

        if get_system() == SystemInfo.Windows:
            expected = paths.find_venv_folder() / "Scripts/pip.exe"
        elif get_system() == SystemInfo.Linux:
            expected = paths.find_venv_folder() / "bin/pip"
        else:
            raise Exception("Not supported os")

        assert venv_pip_path.exists()
        assert venv_pip_path == expected

    def test_find_venv_conan_path(self):
        venv_conan_path = paths.find_venv_conan_path()

        if get_system() == SystemInfo.Windows:
            expected = paths.find_venv_folder() / "Scripts/conan.exe"
        elif get_system() == SystemInfo.Linux:
            expected = paths.find_venv_folder() / "bin/conan"
        else:
            raise Exception("Not supported os")

        assert venv_conan_path.exists()
        assert venv_conan_path == expected

