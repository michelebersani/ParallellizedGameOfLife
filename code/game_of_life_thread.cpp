#include "solvers.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

bool verbose = false;

int binaryInit(){
    return (std::rand())%2;
}

int main(int argc, char * argv[]) {

    if(argc != 6) {
        std::cout << "Usage is: " << argv[0] << " N M number_step seed nw" << std::endl;
        return(-1);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numSteps = atoi(argv[3]);
    std::srand(atoi(argv[4]));
    int numWorkers = atoi(argv[5]);
        
    GameOfLifeRule rule = GameOfLifeRule();
    Board board = Board(N, M, rule);
    ThreadSolver solver = ThreadSolver(&board);
    
    std::vector<std::vector<int>> firstState (N, std::vector<int> (M));
    for (auto &row: firstState){
        std::generate(row.begin(), row.end(), binaryInit);
    }

    board.setInitialState(firstState);
    board.reset();
    solver.solve(numSteps, verbose, numWorkers);
    
    return(0);
}