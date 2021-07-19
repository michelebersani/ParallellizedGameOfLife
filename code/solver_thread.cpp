#include "solvers.h"
#include <thread>

ThreadSolver::ThreadSolver(Board *parent){
    this->parentPtr = parent;
}

void ThreadSolver::solve(int numSteps, bool verbose, int numWorkers, int chunksize){
    utimer tp("thread_solver");
    if(verbose){
        parentPtr->printBoard();
    }

    int N = parentPtr->N;
    int M = parentPtr->M;
    
    for (int i = 0; i < numSteps; i++) {
        
        parentPtr->swapBoards();

        auto threadProcess = [](std::pair<int,int> r, Board * parentPtr){
            for(int i = r.first; i < r.second; i++)
                parentPtr->updateCell(i);
            };

        std::vector< std::pair<int,int> > ranges(numWorkers);                     // vector to compute the ranges 
        int delta { N * M / numWorkers };
        std::vector<std::thread> tids;
        
        for(int i = 0; i < numWorkers; i++) {                     // split the board into peaces
            ranges[i].first = i * delta;
            ranges[i].second   = i != (numWorkers - 1) ? (i + 1) * delta : N * M; 
        }
        
        for(int i = 0; i < numWorkers; i++) {                     // assign chuncks to threads
            tids.push_back(std::thread(threadProcess, ranges[i], parentPtr));
        }
        
        for(std::thread& t: tids) {                        // await thread termination
            t.join();
        }
        if(verbose){
            parentPtr->printBoard();
        }
    }
}