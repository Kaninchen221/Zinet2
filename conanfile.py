from conans import ConanFile, CMake

class ZinetConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = [
        "gtest/1.10.0@",
        "spdlog/1.9.2@",
        "plf_colony/7.06@",
        "glfw/3.3.6@",
        "stb/20200203@",
        "pybind11/2.9.1@",
        "nlohmann_json/3.10.5@",
        "box2d/2.4.1@",
        "glm/0.9.9.5@",
        "glslang/11.7.0@",
        "spirv-tools/2021.4@",
        "shaderc/2021.1@",
        "vulkan-memory-allocator/3.0.1@",
        "glbinding/3.3.0"
        ]
   generators = "cmake"
   default_options = {
        "gtest:shared": True,
        "glfw:shared": True
   }

   def imports(self):
       self.copy(pattern="*.dll", dst="bin", keep_path=False)
       self.copy(pattern="*.dylib", dst="lib", keep_path=False)
       self.copy(pattern="*.pdb", dst="bin", keep_path=False)
