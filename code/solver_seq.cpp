#include "solvers.h"

SequentialSolver::SequentialSolver(Board *parent){
    this->parentPtr = parent;
}

void SequentialSolver::solve(int numSteps, bool verbose, int numWorkers, int chunksize){
    utimer tp("seq_solver");
    if(verbose){
        parentPtr->printBoard();
    }
    for (int i=0; i < numSteps; i++) {
        parentPtr->swapBoards();
        for(int j = 0; j< parentPtr->N * parentPtr->M; j++){
            parentPtr->updateCell(j);
        }
        if(verbose){
            parentPtr->printBoard();
        }
    }
}