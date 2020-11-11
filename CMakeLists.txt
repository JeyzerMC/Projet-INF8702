cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(scene-sous-marine)


set(HEADER_FILES
)
set(SOURCE_FILES
        main.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)


target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
