cmake_minimum_required(VERSION 3.20)
project(chroma-shift-engine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chroma-shift-engine main.cpp include/FrameBuffer.hpp include/ColorMatrix.hpp)
target_compile_options(chroma-shift-engine PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fsanitize=address,undefined,leak
)
target_link_options(chroma-shift-engine PRIVATE
        -fsanitize=address,undefined,leak
)

add_executable(run_test test/test.cpp include/FrameBuffer.hpp include/ColorMatrix.hpp)
target_compile_options(run_test PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fsanitize=address,undefined,leak
)
target_link_options(run_test PRIVATE
        -fsanitize=address,undefined,leak
)