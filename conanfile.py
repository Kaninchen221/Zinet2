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
        "glslang/1.3.239.0@",
        "spirv-tools/1.3.239.0@",
        "shaderc/2023.6@",
        "vulkan-memory-allocator/cci.20231120@"
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
