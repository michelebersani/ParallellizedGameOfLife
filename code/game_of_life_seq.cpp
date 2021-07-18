#include "solvers.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

bool verbose = false;

int binaryInit(){
    return (std::rand())%2;
}

int main(int argc, char * argv[]) {

    if(argc != 5) {
        std::cout << "Usage is: " << argv[0] << " N M number_step seed" << std::endl;
        return(-1);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numSteps = atoi(argv[3]);
    int seed = atoi(argv[4]);
    std::srand(seed);
    short int cellStates = 2;

    GameOfLifeRule rule = GameOfLifeRule();
    Board board = Board(N, M, cellStates, std::ref(rule));
    SequentialSolver solver = SequentialSolver(&board);
    
    std::vector<std::vector<short int>> firstState (N, std::vector<short int> (M));
    for (auto &row: firstState){
        std::generate(row.begin(), row.end(), binaryInit);
    }

    board.defineInitialState(firstState);
    board.reset();
    solver.solve(numSteps, verbose,1);

    return (0);
}