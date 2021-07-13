#!/bin/bash

# script for evaluating 
rm exec/thread.txt exec/seq.txt

#MacOS

g++ -std=c++17  -g   -O3 -finline-functions -DNDEBUG -o exec/seq code/game_of_life_seq.cpp -pthread -Xpreprocessor -fopenmp -lomp
#g++ -std=c++17  -g   -O3 -finline-functions -DNDEBUG -o exec/thread code/game_of_life_thread.cpp -pthread -Xpreprocessor -fopenmp -lomp

#Others

# g++ -o exec/par code/game_of_life_thread.cpp -O3 -pthread
# g++ -o exec/thread code/game_of_life_seq.cpp -O3

# INPUTS: 1 ROWS; 2 COLUMNS; $3 TIMESTEPS; $4 WORKERS; $5 FIXED_SEED_RUNS

echo $1 $2 $3 $4 $5
for ((i = 0 ; i < $5 ; i++)); 
do
   echo $1 $2 $3 $i $4
   #./exec/thread $1 $2 $3 $i $4 >> exec/thread.txt
   ./exec/seq $1 $2 $3 $i # >> exec/seq.txt
done
# python code/mean_std.py