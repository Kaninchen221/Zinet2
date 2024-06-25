from zinet_generator.cmakelists_generator import CMakeListsGenerator
from zinet_generator.cmakelists_generator_target import CMakeListsGeneratorTarget
from pathlib import Path


class CMakeListsGeneratorExecutable(CMakeListsGeneratorTarget):
    def __init__(self) -> None:
        super().__init__()
        self.templatePath = CMakeListsGenerator.get_templates_folder() / "CMakeListsExecutableTemplate.txt"
