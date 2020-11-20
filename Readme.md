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