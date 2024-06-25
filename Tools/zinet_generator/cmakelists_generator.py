import zinet_utilities.paths
from zinet_generator.safe_dict import SafeDict
from pathlib import Path
from zinet_utilities import paths


class CMakeListsGenerator:
    def generate_cmake_implementation(self, templatePath, dictionary):
        template = open(templatePath).read()
        cmakelists = template.format_map(dictionary)
        cmakelists = cmakelists.replace("\\", "/")
        return cmakelists

    def generate_cmakelists(self, dictionary):
        return self.generate_cmake_implementation(self.templatePath, dictionary)

    def prepare_arguments(self):
        return SafeDict()

    @staticmethod
    def get_templates_folder():
        path = (paths.find_zinet_root_path() / "Tools/zinet_generator/templates").absolute()
        if not path.exists():
            raise Exception(f"Can't return valid templates folder \n{path}")
        return path

    fileLocation = Path(__file__)
    templatePath = Path()
    cmdArgs = ''
