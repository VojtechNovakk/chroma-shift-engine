cmake_minimum_required(VERSION 3.20)
project(chroma-shift-engine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(chroma-shift-engine main.cpp include/FrameBuffer.hpp
        include/ColorMatrix.hpp include/Engine.hpp
        include/VideoReader.hpp)

target_include_directories(chroma-shift-engine PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(chroma-shift-engine PRIVATE ${OpenCV_LIBS})

target_compile_options(chroma-shift-engine PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fsanitize=address,undefined,leak
)
target_link_options(chroma-shift-engine PRIVATE
        -fsanitize=address,undefined,leak
)

add_executable(run_test test/test.cpp include/FrameBuffer.hpp
        include/ColorMatrix.hpp include/Engine.hpp
        include/VideoReader.hpp)

target_include_directories(run_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(run_test PRIVATE ${OpenCV_LIBS})

target_compile_options(run_test PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fsanitize=address,undefined,leak
)
target_link_options(run_test PRIVATE
        -fsanitize=address,undefined,leak
)