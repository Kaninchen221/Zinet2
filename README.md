
![Windows Debug Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Windows_Debug.yml/badge.svg)
![Windows Release Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Windows_Release.yml/badge.svg)
![Linux Debug Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Linux_Debug.yml/badge.svg)
![Linux Release Badge](https://github.com/Kaninchen221/Zinet2/actions/workflows/Linux_Release.yml/badge.svg)

# Zinet

## Step by step Windows

+ Install:
	- Python 3.11.4 or newer
	- Visual Studio 17 2022 or newer (Windows)
	- CMake 3.26.4 or newer
+ Clone repo
+ Run `first_time.py`
+ For minimum setup, run the `menu.py` and execute these scripts:
	- Generate Project Files Address Sanitizer Off
	- Conan Install Debug
	- Build Project Debug
	  
+ Project files will be in the build folder

## Runners Info 
I have two self hosted runners:  
+ Windows 10 with Visual Studio 
+ Linux Debian with GNU

## FAQ
  + If you have a strange problem with the "Conan Install" step, then try "conan remove --locks" command
    
