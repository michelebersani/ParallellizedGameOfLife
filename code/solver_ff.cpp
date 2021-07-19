#include "solvers.h"
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace ff;

ffSolver::ffSolver(Board *parent){
    this->parentPtr = parent;
}

void ffSolver::solve(int numSteps, bool verbose, int numWorkers, int chunksize){
    utimer tp("ff_solver");
    int size = parentPtr->N * parentPtr->M;
    
    ParallelFor pf(numWorkers);

    if(verbose){
        parentPtr->printBoard();
    }

    for(int i = 0; i < numSteps; i++){
        
        parentPtr->swapBoards();

        pf.parallel_for(0, size, 1, chunksize, [this](const int j) {
                this->parentPtr->updateCell(j);
            }, numWorkers);
        
        if(verbose){
            parentPtr->printBoard();
        }
    }
}