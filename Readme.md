# How to compile in 3 simple steps
1. Init vcpkg, the package manager
    ```
    $ git submodule update --init
    $ ./vcpkg/bootstrap-vcpkg.sh
    ```
    Note : use `./vcpkg/bootstrap-vcpkg.bat` if you are on windows. More info [here](https://github.com/microsoft/vcpkg#getting-started).

2. Install dependencies
    ```
   $ ./vcpkg/vcpkg install glfw3 glm spdlog stb glad assimp
   ```

3. Build the project using `cmake` (or open it in your IDE if it supports cmake)
    ```
   $ mkdir ./build
   $ cd build
   $ cmake ../
   $ make .
   ```
   Note: if you are on windows, there is a bug with visual studio preventing the generation of the CMake project. You can use cmake separatly to generate the project and open it with visual studio.Note: if you are on windows, there is a bug with visual studio preventing the generation of the CMake project. You can use cmake separatly to generate the project and open it with visual studio.Note: if you are on windows, there is a bug with visual studio preventing the generation of the CMake project. You can use cmake separatly to generate the project and open it with visual studio.


# Inputs

W -> Move Forward
S -> Move Backward
A -> Move Left
D -> Move Right
Q -> Move Down
E -> Move Up

Mouse -> Turn camera

R -> Hot-Reload Shader

V -> Toggle Toon Shading
C -> Toggle Caustics
B -> Toggle Edges
N -> Toggle Normal Smoothing
M -> Toggle Wobbling
J -> Toggle Blur
K -> Toggle Underwater Tint
L -> Toggle Watercolor Textures
O -> Toggle Erosion
P -> Toggle Dilation

1 -> Show Selected Effects
2 -> Show All Effects
3 -> Show No Effects
