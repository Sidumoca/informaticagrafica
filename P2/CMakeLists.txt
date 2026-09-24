cmake_minimum_required(VERSION 3.16)

project(images LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PNG REQUIRED)
find_package(OpenEXR REQUIRED)

add_executable(images
    mainImagenes.cpp
    image.cpp
)

target_link_libraries(images PRIVATE PNG::PNG OpenEXR::OpenEXR)

if(MSVC)
    target_compile_options(images PRIVATE /W4)
else()
    target_compile_options(images PRIVATE -Wall -Wextra -Wpedantic)
endif()