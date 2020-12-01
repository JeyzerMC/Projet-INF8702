# How to compile in 3 simple steps
1. Init vcpkg, the package manager
    ```
    $ git submodule update --init
    $ ./vcpkg/bootstrap-vcpkg.sh
    ```
    Note : use `./vcpkg/bootstrap-vcpkg.bat` if you are on windows. More info [here](https://github.com/microsoft/vcpkg#getting-started).

2. Install dependencies
    ```
   $ ./vcpkg/vcpkg install glew glfw3 glm spdlog tinyobjloader stb glad
   ```

3. Build the project using `cmake` (or open it in your IDE if it supports cmake)
    ```
   $ mkdir ./build
   $ cd build
   $ cmake ../
   $ make .
   ```


# Inputs

W -> Move Forward
S -> Move Backward
A -> Move Left
D -> Move Right
Q -> Move Down
E -> Move Up

Mouse -> Turn camera

R -> Reload shader

V -> Toggle Toon Shading
C -> Toggle Caustics
B -> Toggle Edges
N -> Toggle Normal Smoothing
M -> Toggle Wobbling
J -> Toggle Blur
K -> Toggle Underwater Tint

1 -> Show Selected Effects
2 -> Show All Effects
3 -> Show No Effects