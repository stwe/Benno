# Benno

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/e702220c4290499091156333bcecf168)](https://www.codacy.com/gh/stwe/Benno/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=stwe/Benno&amp;utm_campaign=Badge_Grade)
[![Build Status](https://travis-ci.org/stwe/Benno.svg?branch=master)](https://travis-ci.org/stwe/Benno)

<img src="https://github.com/stwe/Benno/blob/master/Benno/res/devLog/20112020.png" width="250" height="200" />

## Supported versions

The project works with Anno 1602: 

- NINA ("Neue Inseln, Neue Abenteuer")
- History Edition

## Requirements

- [SDL2](https://www.libsdl.org/download-2.0.php)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](https://glm.g-truc.net/0.9.9/index.html)
- [spdlog](https://github.com/gabime/spdlog)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [nlohmann_json](https://github.com/nlohmann/json)
- C++ 17 Compiler

## Platforms

- Windows
- Linux

## Build instructions

### Getting Started

First, [install](https://docs.conan.io/en/latest/installation.html) the Conan package manager locally. Then, adding the Bincrafters repository as a Conan Remote.

```bash
$ conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

If you are using GCC compiler >= 5.1, Conan will set the `compiler.libcxx` to the old ABI for backwards compatibility. You can change this with the following commands:

```bash
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
```

As this project relies on multiple 3rd-Party Libs, I created a `conanfile_premake.txt` and a `conanfile_cmake.txt` with all the requirements.

```txt
[requires]
sdl2/2.0.12@bincrafters/stable
glew/2.1.0@bincrafters/stable
glm/0.9.9.8
spdlog/1.8.0
imgui/1.79
nlohmann_json/3.9.1

[generators]
premake
```

```txt

# ...

[generators]
cmake
```

### Windows

My favor build configuration tool for Windows is Premake5. Premake5 can generate Makefiles and Visual Studio Solutions with a single description file for cross-platform projects. Download the [Premake5 repository](https://premake.github.io/download.html) to your preferred location.

Complete the installation of requirements for the project running:

```bash
$ conan install conanfile_premake.txt -s build_type=Debug
```

or

```bash
$ conan install conanfile_premake.txt -s build_type=Release
```

```bash
$ premake5 vs2019
```

### Linux

Complete the installation of requirements for the project running:

```bash
$ conan install conanfile_cmake.txt -s build_type=Debug --build missing
```

or

```bash
$ conan install conanfile_cmake.txt -s build_type=Release --build missing
```

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## License

Benno is licensed under the GPL-2.0 License, see [LICENSE](https://github.com/stwe/Benno/blob/master/LICENSE) for more information.
