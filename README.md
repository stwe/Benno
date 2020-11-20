# Benno

<img src="https://github.com/stwe/Benno/blob/master/Benno/res/devLog/20112020.png" width="250" height="200" />

## Supported versions



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

On Linux, it's probably easiest to get all of the dependencies with `sudo apt install` and use the CMakeLists.txt files attached to this project. I suggest the following guide, which use **Conan** and **Premake5**.

### Getting Started

First, [install](https://docs.conan.io/en/latest/installation.html) the Conan package manager locally. Then, adding the Bincrafters repository as a Conan Remote.

```bash
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

My favor build configuration tool: Premake5. Premake5 can generate Makefiles and Visual Studio Solutions with a single description file for cross-platform projects. Download the [Premake5 repository](https://premake.github.io/download.html) to your preferred location.

As this project relies on multiple 3rd-Party Libs, I created a `conanfile.txt` with all the requirements.

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

### Windows

Complete the installation of requirements for the project running:

```bash
conan install . -s build_type=Debug --build=force
```

or

```bash
conan install . -s build_type=Release --build=force
```

```bash
premake5 vs2019
```

### Linux

Complete the installation of requirements for the project running:

```bash
conan install . -s build_type=Debug --build=sdl2
```

or

```bash
conan install . -s build_type=Release --build=sdl2
```

```bash
premake5 gmake
```

```bash
make -j8
```

## License

Benno is licensed under the GPL-2.0 License, see [LICENSE](https://github.com/stwe/Benno/blob/master/LICENSE) for more information.
