from zinet_generator.cmakelists_generator import CMakeListsGenerator
from pathlib import Path
import numpy


class ProjectGenerator:

    @staticmethod
    def collect_recipes(path):
        print("Find recipes in path: " + str(path))
        paths = ProjectGenerator.collect_files(path, 'recipe_*.py')
        return paths

    def execute_recipe(self, path):
        args = {
            "project_generator": self
        }
        print("Exec recipt: " + str(path))
        exec(open(path).read(), args)

    def generate_project(self, project_root_path, cmd_args):
        self._collectedRecipes = self.collect_recipes(project_root_path)
        print("Collected recipes: ", self._collectedRecipes.size)

        for recipePath in self._collectedRecipes:
            self.execute_recipe(recipePath)
            if self._generators.size == 0:
                raise RuntimeError('No valid generators in collected recipes')

            generator = self._generators[-1]
            generator.fileLocation = recipePath
            generator.cmdArgs = cmd_args
            folder = recipePath.parent
            arguments = generator.prepare_arguments()
            cmakelists = generator.generate_cmakelists(arguments)
            cmake_lists_path = folder / "CMakeLists.txt"
            file = open(cmake_lists_path, "w")
            file.write(cmakelists)
            file.close()

    @staticmethod
    def collect_files(path, format):
        paths = path.rglob(format)
        return numpy.fromiter(paths, Path)

    def add_generator(self, generator):
        self._generators = numpy.append(self._generators, generator)

    def get_generators(self):
        return self._generators

    def get_collected_recipes(self):
        return self._collectedRecipes

    _generators = numpy.empty(0, CMakeListsGenerator)
    _collectedRecipes = numpy.empty(0, Path)
