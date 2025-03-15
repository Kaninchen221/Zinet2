from conan import ConanFile
from conan.tools import cmake
from conan.tools.files import copy
from pathlib import Path

from Tools.zinet_utilities.platform_info import *

class ZinetConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "gtest/1.15.0@",
        "spdlog/1.14.1@",
        "plf_colony/7.06@",
        "glfw/3.4@",
        "stb/cci.20240531@",
        "pybind11/2.9.1@",
        "nlohmann_json/3.10.5@",
        "glm/0.9.9.5@",
        "glslang/1.3.239.0@",
        "spirv-tools/1.3.239.0@",
        "shaderc/2023.6@",
        "vulkan-memory-allocator/cci.20231120@"
        ]
    generators = ["CMakeToolchain", "CMakeDeps"]
    default_options = {
        "gtest/*:shared": True,
        "glfw/*:shared": True
    }

    if get_system() == SystemInfo.Linux:
        default_options["glfw/*:with_wayland"] = True

    def generate(self):
        for dep in self.dependencies.values():
            
            if self.settings.compiler == "gcc":
                bin_path = Path(self.source_folder) / "build" / "bin"
                lib_ext = "*.so"
            elif self.settings.compiler == "msvc":
                bin_path = Path(self.source_folder) / "build" / "bin" / str(self.settings.build_type)
                lib_ext = "*.dll"
                
            if dep.cpp_info.bindirs:
                copy(self, lib_ext, src=dep.cpp_info.bindirs[0], dst=bin_path, keep_path=False)
                
            if dep.cpp_info.libdirs:
                copy(self, lib_ext, src=dep.cpp_info.libdirs[0], dst=bin_path, keep_path=False)