from conan import ConanFile
from conan.tools import cmake

class ZinetConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = [
        "cmake/3.30.1@",
        "gtest/1.15.0@",
        "spdlog/1.9.2@",
        "plf_colony/7.06@",
        "glfw/3.3.6@",
        "stb/cci.20240531@",
        "pybind11/2.9.1@",
        "nlohmann_json/3.10.5@",
        "glm/0.9.9.5@",
        "glslang/1.3.239.0@",
        "spirv-tools/1.3.239.0@",
        "shaderc/2023.6@",
        "vulkan-memory-allocator/cci.20231120@"
        ]
   generators = "cmake"
   default_options = {
        "gtest/*:shared": True,
        "glfw/*:shared": True
   }

   def imports(self):
       self.copy(pattern="*.dll", dst="bin", keep_path=False)
       self.copy(pattern="*.dylib", dst="lib", keep_path=False)
       self.copy(pattern="*.pdb", dst="bin", keep_path=False)
