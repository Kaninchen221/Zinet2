from zinet_utilities import paths


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

    def test_find_scripts_folder(self):
        actual = paths.find_scripts_folder()
        expected = paths.find_zinet_root_path() / "Scripts"
        assert actual == expected

    def test_find_conan_profiles_folder(self):
        actual = paths.find_conan_profiles_folder()
        expected = paths.find_scripts_folder() / "conan_profiles"
        assert actual == expected
