

![Windows Debug Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Windows_Debug.yml/badge.svg)
![Windows Release Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Windows_Release.yml/badge.svg)
![Linux Debug Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Linux_Debug.yml/badge.svg)
![Linux Release Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Linux_Release.yml/badge.svg)

# Zinet
Readme last update: 31/8/2025
Zinet is a game engine focused mainly on 2D graphics
The purpose of this project is learning the C++ language, maintaining a big project and in the feature creating a game

## Step by step Windows

+ Requirements:
	- Python 3.13.5 or newer
	- Visual Studio 17 2022 (17.14.9) or newer (Windows)
	- CMake 3.26.4 or newer
+ Install:
	- Clone repo
	- Run `first_time.py`
	- For minimum setup, run the `menu.py` and execute these scripts:
		+ Generate Project Files Address Sanitizer Off
		+ Conan Install Debug
		+ Build Project Debug
	- Project files are in the `build` folder
	- You can compile and run automatic tests from this program 

## Architecture
+ For dependencies the project is using conan 2
	- ImGui is an exception for this rule. It's located under the `Libs/ThirdParty` 
+ The project consists mainly of C++ and Python code
+ Every library or excecutable project is in the `Libs` folder and consists at least a  one additional executable project for `Unit Tests`
+ More complicated libraries consists of an additional executable project `Manual Tests` 
+ `Google Test` for tests
+ `ImGui` for the editor UI
+ `Vulkan` for the renderer
+ `VMA` for allocating memory in the vulkan renderer

## Runners Info 
I have two self hosted runners:  
- Windows 10 with Visual Studio 
- Linux Debian with GNU (Inactive)

## FAQ
  + If you have a strange problem with the "Conan Install" step, then try "conan remove --locks" command
    
