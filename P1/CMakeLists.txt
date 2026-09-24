cmake_minimum_required(VERSION 3.16)

project(planet_trajectory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED NO_MODULE)

add_executable(planet
    mainPlanetas.cpp
    pd.cpp
    planet.cpp
)

target_link_libraries(planet PRIVATE Eigen3::Eigen)

if(MSVC)
    target_compile_options(planet PRIVATE /W4)
else()
    target_compile_options(planet PRIVATE -Wall -Wextra -Wpedantic)
endif()