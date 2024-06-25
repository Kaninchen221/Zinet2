from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.safe_dict import SafeDict
from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from pathlib import Path


class CMakeListsGeneratorLibrary(CMakeListsGeneratorTarget):

    def __init__(self) -> None:
        super().__init__()
        self.templatePath = CMakeListsGenerator.get_templates_folder() / "CMakeListsLibraryTemplate.txt"

    def prepare_arguments(self):
        testsSubfolders = ""
        for path in Path(self.fileLocation.parent).iterdir():
            if path.is_dir() and path.name.startswith(self.testsSubfoldersPrefix):
                testsSubfolders += "add_subdirectory(" + path.name + ")\n\t"

        result = super().prepare_arguments()
        result |= SafeDict(
            argument_library_type = self.libraryType,
            argument_should_add_tests = self.shouldAddTests,
            argument_tests_subfolders = testsSubfolders
        )
        return result
        
    libraryType = "STATIC"
    shouldAddTests = "TRUE"
    testsSubfoldersPrefix = "tests_"
