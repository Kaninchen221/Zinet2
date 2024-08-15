from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.safe_dict import SafeDict
from pathlib import Path

from zinet_utilities.paths import find_tools_folder


class TestCmakelistsGeneratorTarget:
    def test_inheritance(self):
        assert issubclass(CMakeListsGeneratorTarget, CMakeListsGenerator)

    def test_properties(self):
        assert self.generatorTarget.targetName == "target_name"
        assert self.generatorTarget.targetPrettyName == "target_pretty_name"
        assert self.generatorTarget.headersExtension == "hpp"
        assert self.generatorTarget.headersSubfolder == "include"
        assert self.generatorTarget.sourcesExtension == "cpp"
        assert self.generatorTarget.sourcesSubfolder == "source"
        assert self.generatorTarget.cppVersion == "cxx_std_20"
        assert self.generatorTarget.includeDirectories == ""
        assert self.generatorTarget.linkLibraries == ""
        assert self.generatorTarget.compileDefinitions == ""
        assert self.generatorTarget.archiveOutputSubfolder == "archive"
        assert self.generatorTarget.libraryOutputSubfolder == "lib"
        assert self.generatorTarget.runtimeOutputSubfolder == "runtime"

    def test_prepare_arguments(self):
        self.generatorTarget.includeDirectories = "includeDirectories"
        self.generatorTarget.linkLibraries = "linkLibraries"
        self.generatorTarget.compileDefinitions = "compileDefinitions"

        test_files_path = find_tools_folder() / r"tests/zinet_generator/test_files"
        paths = [test_files_path / self.generatorTarget.headersSubfolder]
        extensions = [self.generatorTarget.headersExtension]
        headers_subfolder = self.generatorTarget.headersSubfolder
        sources_subfolder = self.generatorTarget.sourcesSubfolder
        expected_files_argument = ("/" + str(test_files_path / headers_subfolder / "header1.hpp\"") + "\n\t\""
                                   + str(test_files_path / headers_subfolder / "header2.hpp") + "\"\n\t\n\t")
        expected_files_argument += ("/" + str(test_files_path / sources_subfolder / "source1.cpp\"") + "\n\t\""
                                    + str(test_files_path / sources_subfolder / "source2.cpp") + "\"\n\t\n\t")

        self.generatorTarget.fileLocation = test_files_path / "test_files"

        arguments = self.generatorTarget.prepare_arguments()
        assert len(self.generatorTarget.files) == len(expected_files_argument)
        assert type(arguments) is SafeDict
        assert arguments['argument_target_name'] == self.generatorTarget.targetName
        assert arguments['argument_target_pretty_name'] == self.generatorTarget.targetPrettyName
        assert arguments['argument_headers_subfolder'] == self.generatorTarget.headersSubfolder
        assert arguments['argument_sources_subfolder'] == self.generatorTarget.sourcesSubfolder
        assert arguments['argument_files'] == self.generatorTarget.files
        assert arguments['argument_cpp_version'] == self.generatorTarget.cppVersion
        assert arguments['argument_include_directories'] == self.generatorTarget.includeDirectories
        assert arguments['argument_link_libraries'] == self.generatorTarget.linkLibraries
        assert arguments['argument_compile_definitions'] == self.generatorTarget.compileDefinitions
        assert arguments['argument_archive_output_subfolder'] == self.generatorTarget.archiveOutputSubfolder
        assert arguments['argument_library_output_subfolder'] == self.generatorTarget.libraryOutputSubfolder
        assert arguments['argument_runtime_output_subfolder'] == self.generatorTarget.runtimeOutputSubfolder

    generatorTarget = CMakeListsGeneratorTarget()
