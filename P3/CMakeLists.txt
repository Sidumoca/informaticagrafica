cmake_minimum_required(VERSION 3.16)

project(main3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(PNG REQUIRED)
find_package(OpenEXR REQUIRED)

add_executable(main3
main3.cpp
pd.cpp
plano.cpp
esfera.cpp
ray.cpp
image.cpp
camara.cpp
)

target_link_libraries(main3 PRIVATE Eigen3::Eigen PNG::PNG OpenEXR::OpenEXR)

if(MSVC)
target_compile_options(main3 PRIVATE /W4)
else()
target_compile_options(main3 PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()
