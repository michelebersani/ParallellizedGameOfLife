#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "solvers.h"

bool verbose = false;

int binaryInit(){
    return (std::rand())%2;
}

int main(int argc, char * argv[]) {

    if(argc != 7) {
        std::cout << "Usage is: " << argv[0] << " N M number_step seed nw dynamic_assignment" << std::endl;
        return(-1);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numSteps = atoi(argv[3]);
    std::srand(atoi(argv[4]));
    int numWorkers = atoi(argv[5]);
    int dynamic = atoi(argv[6]);

    short int cellStates = 2;

    GameOfLifeRule rule = GameOfLifeRule();
    Board board = Board(N, M, cellStates, rule);
    std::vector<std::vector<short int>> firstState (N, std::vector<short int> (M));
    for (auto &row: firstState){
        std::generate(row.begin(), row.end(), binaryInit);
    }
    board.defineInitialState(firstState);
    board.reset();

    if(dynamic){
        ffFarmSolver solver = ffFarmSolver(&board);
        solver.solve(numSteps, verbose, numWorkers);
    }
    else{
        ffParforSolver solver = ffParforSolver(&board);
        solver.solve(numSteps, verbose, numWorkers);
    }
    return(0);
}