# Double-Pendulum
Double Pendulum Simulation using C++20 and SFML

### Description
This Project has been made solely for the purpose of learning more about SFML and C++20 and simulations. Although it implements C++20, lot of the code and syntax still use older C++ styles like raw pointers. In future releases, the code will be gradually updated to more modern C++ style and syntax and also to SFML-3.0.

Currently there is no on screen gui for manipulating the pendulums' length and mass or changing gravity and more fun stuff, you can still do it from the code, however in future you can expect support for that with ImGUI-SFML.

The integrator used in the simulation is Runge-Kutta 4th order which is not very stable so the simulation gradually loses energy with time with the added cost of being compute intensive. This shall also be modified and replaced with Verlet integration which is more stable.

P.S.- I am extremely new to C++ build sytems and it is a nightmare to deal with so if the CMake scripts don't work for your end please raise an issue and if you found any solution to mitigate the above create a pull request, shall try to review and merge it as soon as possible.

### Requirements
1. `g++ version >= 10`:

    Check your compiler version by running
    ```bash
    $ g++ --version 
    ``` 
    in your terminal, in case you have older compiler you will need a new one to compile this project.
2. `cmake >= 3.21`:

    Check if you have Cmake installed by running
    ```bash
    $ cmake --version
    ```
    If it is not available then install CMake from [here](https://cmake.org/download/ "Download CMake here") depending on your OS. Add it to your PATH environment variable.
3. `SFML = 2.6.x`:

    The provided CMakeLists.txt by default installs SFML for you using `FetchContent()` in the build directory you provide and links them to the executable. If you already have SFML installed with version 2.6 in standard path then you can disable this behaviour and cmake will do the rest
4. `Ninja`:

    Check if you have Ninja installed by running
    ```bash
    $ ninja --version
    ```
    If it is not available then install Ninja from [here](https://github.com/ninja-build/ninja/releases "Download Ninja here") depending on your OS. Add it to your PATH environment variable.

### How to install
1. Create a `build` directory in current directory and then create a Debug and Release directoy under it.

    ```bash
    $ mkdir build/Debug && mkdir build/Release
    ```
Select Debug or Release build: 

2. For a Release build:    

    ```cmake
    $ cmake -G "Ninja" -S . -B build/Release
    $ cmake --build build/Release --config Release
    ```
3. For a Debug build:

    ```cmake
    $ cmake -G "Ninja" -S . -B build/Debug
    $ cmake --build build/Release --config Debug
    ```
4. Navigate to `build/Debug` or `build/Release` and run the executable

    ```bash
    $ cd build/Release[Debug]
    $ ./DoublePendulum (Linux) ./DoublePendulum.exe(Windows)
    ```
That's it!
_If you like this project be sure to star it_ :)
