#include<iostream>
#include <thread>
#include <unistd.h>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "utimer.cpp"
#include"board.h"

using namespace ff;

void Solver::setParent(Board * parent){
    parentPtr = parent;
}

void SequentialSolver::operator() (int numSteps, bool verbose, int numWorkers){
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

void ffSolver::operator() (int numSteps, bool verbose, int numWorkers){
    utimer tp("ff_solver");
    long size = parentPtr->N * parentPtr->M;
    int chunk = 0;
    
    if(verbose){
        parentPtr->printBoard();
    }
    for(int i = 0; i < numSteps; i++){
        
        parentPtr->swapBoards();

        parallel_for_idx(0, size, 1, chunk, [&](const long start, const long stop, const long thid) {
                for(long i=start;i<stop;++i)
                    parentPtr->updateCell(i);
            }, numWorkers);
        
        if(verbose){
            parentPtr->printBoard();
        }
    }
}

void ThreadSolver::operator() (int numSteps, bool verbose, int numWorkers){
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
            ranges[i].second   = (i != (numWorkers - 1) ? (i + 1) * delta : N * M); 
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

int GameOfLifeRule::operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight){

    int sum = up + right + left + down + downLeft + downRight + upLeft + upRight; 
    
    if(sum == 3 || (centre == 1 && sum == 2)){
        return 1;
    }
    else{
        return 0;
    }
}

Board::Board(int N, int M, Solver &solver, UpdateRule &rule) : N(N), M(M), solver(solver), rule(rule){
    initialState = std::vector<int> (N*M);
    presentBoard = std::vector<int> (N*M);
    pastBoard = std::vector<int> (N*M);
    solver.setParent(this);
}

void Board::setUpdateRule(UpdateRule& rule){
    this->rule = rule;
}

void Board::setInitialState(std::vector<std::vector<int>> &matrix){
    initialState = std::vector<int>();
    for(auto && v : matrix){
        initialState.insert(initialState.end(), v.begin(), v.end());
    }
}

void Board::reset(){
    presentBoard = initialState;
}

void Board::solve(int numSteps, bool verbose, int numWorkers){
    solver(numSteps, verbose, numWorkers);
}

void Board::swapBoards(){
    presentBoard.swap(pastBoard);
}

void Board::updateCell(int j){

    int row = j/M;
    int col = j%M;

    int upIndex = (row == 0) ? (N - 1) * M + col : (row - 1) * M + col;
    int downIndex = (row == N - 1) ? col : (row + 1) * M + col;
    int leftIndex = (col == 0) ? (row + 1) * M - 1 : row * M  + col - 1;
    int rightIndex = (col == M - 1) ? row *M : row * M + col + 1;
    int upLeftIndex = upIndex - col + leftIndex % M; 
    int upRightIndex= upIndex - col + rightIndex % M; 
    int downLeftIndex = downIndex - col + leftIndex % M;
    int downRightIndex = downIndex - col + rightIndex % M;
    
    int centre = pastBoard[j];
    int up = pastBoard[upIndex];
    int down = pastBoard[downIndex];
    int left = pastBoard[leftIndex];
    int right = pastBoard[rightIndex];
    int upLeft = pastBoard[upLeftIndex];
    int upRight = pastBoard[upRightIndex];
    int downLeft = pastBoard[downLeftIndex];
    int downRight = pastBoard[downRightIndex]; 
    
    presentBoard[j] = rule(centre, up, down, left, right, upLeft, upRight, downLeft, downRight);
}

void Board::printBoard(){

    for (int i=0; i<M + 2; ++i){
        std::cout<<"\r";
    }

    for (int i=0; i<M; ++i){
        std::cout<<"-";
    }

    std::cout<<"\n";
    
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            int x = presentBoard[j + M*i];
            if(x == 0){
                std::cout<<" ";
            }
            else{
                std::cout<<x;
            }
        }
        std::cout<<"\n";
    }

    for (int i=0; i<M; ++i){
        std::cout<<"-";
    }
    std::cout<<"\n";
    sleep(1);
}