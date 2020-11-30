cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(scene-sous-marine)

set(HEADER_FILES
        utils/camera.h
        utils/shader.h
        utils/mesh.h
        utils/model.h
        utils/constants.h
        src/texture.h
        src/post_processing.h
        src/scene.h
        src/shadowmap.h
        src/caustics.h
        src/transform.h
)
set(SOURCE_FILES
        main.cpp
        src/texture.cpp
        src/stb_impl.cpp
        src/post_processing.cpp
        src/scene.cpp
        src/shadowmap.cpp
        src/caustics.cpp
        src/transform.cpp
        utils/shader.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(assimp CONFIG REQUIRED)
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIBRARIES})
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
