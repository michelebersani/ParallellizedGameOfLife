cmake_minimum_required(VERSION 3.10)
project(CellularAutomata VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -finline-functions -DNDEBUG -I ../libraries/fastflow -pthread")

add_library(
    seq_solver
    ../code/solvers.h
    ../code/solver_seq.cpp
    )
add_library(
    ff_farm_solver
    ../code/solvers.h
    ../code/solver_ff_farm.cpp
)
add_library(
    ff_parfor_solver
    ../code/solvers.h
    ../code/solver_ff_parfor.cpp
)
add_library(
    thread_solver
    ../code/solvers.h
    ../code/solver_thread.cpp
)
add_library(
    thread_pool_solver
    ../code/solvers.h
    ../code/solver_dyn_thread.cpp
)
add_library(
    board
    ../code/board.h
    ../code/board.cpp
)
add_library(
    rules
    ../code/rules.h
    ../code/rules.cpp
)

add_executable(seq ../code/game_of_life_seq.cpp)
target_link_libraries(seq PRIVATE seq_solver rules board)

add_executable(ff ../code/game_of_life_ff.cpp)
target_link_libraries(ff PRIVATE ff_parfor_solver ff_farm_solver rules board)

add_executable(thread ../code/game_of_life_thread.cpp)
target_link_libraries(thread PRIVATE thread_solver rules board)

add_executable(dyn_thread ../code/game_of_life_dyn_thread.cpp)
target_link_libraries(dyn_thread PRIVATE thread_pool_solver rules board)