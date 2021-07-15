cmake_minimum_required(VERSION 3.10)
project(CellularAutomata VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 17)

add_compile_options(-g -O3 -finline-functions -DNDEBUG -I ../libraries/fastflow -o -pthread -Xpreprocessor -fopenmp -lomp)

add_library(
    board
    ../code/board.h
    ../code/board.cpp
    )
add_executable(seq ../code/game_of_life_seq.cpp)
add_executable(ff ../code/game_of_life_ff.cpp)
add_executable(thread ../code/game_of_life_thread.cpp)

target_link_libraries(seq PRIVATE board)
target_link_libraries(ff PRIVATE board)
target_link_libraries(thread PRIVATE board)