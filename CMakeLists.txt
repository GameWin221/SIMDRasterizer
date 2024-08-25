cmake_minimum_required(VERSION 3.13)
project(simd_experiment)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/raster.hpp
    src/raster.cpp

    src/math/math.hpp

    src/math/mat.hpp
    src/math/mat4.cpp

    src/math/vec.hpp
    src/math/vec2.cpp
    src/math/vec3.cpp
    src/math/vec4.cpp

    src/math/scalar.hpp
    src/math/trigonometry.hpp
    src/math/math_config.hpp
    src/ply_importer.hpp
    src/ply_importer.cpp
)

set(MINIFB_BUILD_EXAMPLES FALSE)
add_subdirectory(external/minifb)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -mavx -mavx2 -msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -g)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O3)
    endif()

    target_link_options(${PROJECT_NAME} PRIVATE -static -pthread)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE /DEBUG)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE /O2 /Qpar)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE minifb)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res/ ${PROJECT_BINARY_DIR}/res/
)
message(STATUS "${PROJECT_NAME}: Copied ${CMAKE_CURRENT_LIST_DIR}/res/ to ${PROJECT_BINARY_DIR}/res/")