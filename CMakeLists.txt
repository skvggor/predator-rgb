cmake_minimum_required(VERSION 3.21)
project(predator-rgb LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Quick QuickControls2)
find_package(GTest REQUIRED)

qt_policy(SET QTP0004 NEW)

enable_testing()

add_subdirectory(core)
add_subdirectory(backend)
add_subdirectory(app)
add_subdirectory(tests)
