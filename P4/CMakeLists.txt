cmake_minimum_required(VERSION 3.16)

project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(PNG REQUIRED)
find_package(OpenEXR REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(main
main.cpp
geometria/pd.cpp
primitivas/plano.cpp
primitivas/esfera.cpp
geometria/ray.cpp
image.cpp
camara.cpp
primitivas/monolito.cpp
primitivas/triangulo.cpp
)

target_link_libraries(main PRIVATE Eigen3::Eigen PNG::PNG OpenEXR::OpenEXR OpenMP::OpenMP_CXX)

if(MSVC)
target_compile_options(main PRIVATE /W4)
else()
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()
