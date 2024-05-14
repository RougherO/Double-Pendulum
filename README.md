# Double-Pendulum
Double Pendulum Simulation using C++20 and SFML

![example-image](image.png)

## Description
This Project has been made solely for the purpose of learning more about SFML and C++20 and simulations. The integrator used is a modified version of Velocity Verlet where acceleration also depends on velocity [see here](https://gamedev.stackexchange.com/a/41917). The UI has been made using [Dear ImGui](https://github.com/ocornut/imgui) and [ImGui-SFML](https://github.com/SFML/imgui-sfml) for sfml bindings of imgui.

#### Current features :
* ~~CLI - interface to provide initial conditions for the pendulum~~ CLI support has been removed completely.
* ~~No GUI support for manipulation of the paramenters~~ Fear not Dear, ImGui is here ;)
* ~~Uses RK4 integrator~~ Uses modified Velocity Verlet

#### Future releases :
* Add options to allow damping effect
* Mulitple pendulums
* Anchor points to grab the pendulum in the UI itself
* _Feel free to open a PR for more_

> P.S.- ~~I am extremely new to C++ build sytems and it is a nightmare to deal with,~~ I have come a long way since the first time I made this so I am somewhat more comfortable and confident now in building C++ projects however cross IDE CMake scripts are still not my cup of tea so if the CMake scripts don't work for your end please raise an issue and if you find any solution to mitigate the above create a PR.

<!-- ## How to install -->
<!-- If you are on Windows you can directly go to [Release](https://github.com/RougherO/Double-Pendulum/releases/tag/v1.0.0) section and choose the Windows zip folder extract it and execute the exe file from console. See [How to use](#how-to-use)

It is unfortunate that I do not have enough knowledge to distribute projects in the form of AppImage or Flatpaks for Linux at this point so building from source is recommended to prevent any dependency issue. You can expect the next release to have a Linux AppImage version. -->

<!-- For Linux folks [building the project](#building) is the recommended route for now. -->
## How to build
After cloning the project, make sure to initialize the submodules too.
```bash
$ git clone --recursive <REPO_LINK>
```
If project is already cloned
```bash
$ git submodule update --init --recursive
```
### Requirements
1. C++ compiler which supports `C++20 and above`: Check your compiler version, if it has support for c++20.
2. `CMake >= 3.21`: Check if you have CMake installed by running
    ```bash
    $ cmake --version
    ```
    If it is not available then install CMake from [here](https://cmake.org/download/) depending on your OS.

    If you are on Linux, you can use the one provided by your package managers.

### Building
1. Create a `build` directory in current project directory

    ```bash
    $ mkdir build
    ```
2. Generate build files in the `build` directory

    ```bash
    $ cmake -S . -B build
    ```

3. * For a Release build:    

    ```bash
    $ cmake --build build --config Release
    ```
    * For a Debug build:

    ```bash
    $ cmake --build build --config Debug
    ```
    This will build for the default builder you have [Makefile or Ninja or MSVS etc]. You could specify a builder by adding a -G flag, for example to create build files for Ninja:
    ```bash
    $ cmake -G "Ninja" -S . -B build/[Debug/Release]
    ```
4. Navigate to `build` and run the executable

    ```bash
    $ cd build
    $ ./DoublePendulum (Linux) ./DoublePendulum.exe(Windows)
    ```

_If you like this project be sure to star it_ :)
